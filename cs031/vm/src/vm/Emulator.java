package vm;

import java.io.FileInputStream;

import visualizer.Visualizer;
import vmsupport.*;

public class Emulator {
	public final static int DEFAULT_NBR_PAGES = 16;

	public static void usageError ()
	{
		System.err.println("Usage: java vm.Emulator [-debug] [-v] [-lru | -lfu | -random | -special] [-pages nbrpages] [-input file] exefile");
		System.exit(2);
	}

	public static void main (String[] args) throws Exception
	{
		int			nbrPages = DEFAULT_NBR_PAGES;
		int			optind = 0;
		String			stdinFile = null;
		boolean			debugMode = false;
		boolean			runVisualizer = false;
		try {
			while (optind < args.length && args[optind].charAt(0) == '-') {
				if (args[optind].equals("-debug")) {
					debugMode = true;
					++optind;
				} else if (args[optind].equals("-v")) {
					runVisualizer = true;
					++optind;
				} else if (args[optind].equals("-lru")) {
					MyMMU.policy = ReplacementPolicy.LRU;
					++optind;
				} else if (args[optind].equals("-lfu")) {
					MyMMU.policy = ReplacementPolicy.LFU;
					++optind;
				} else if (args[optind].equals("-random")) {
					MyMMU.policy = ReplacementPolicy.Random;
					++optind;
				} else if (args[optind].equals("-special")) {
					MyMMU.policy = ReplacementPolicy.Special;
					++optind;
				} else if (args[optind].equals("-pages") && optind + 1 < args.length) {
					nbrPages = Integer.parseInt(args[optind + 1]);
					optind += 2;
				} else if (args[optind].equals("-input") && optind + 1 < args.length) {
					stdinFile = args[optind + 1];
					optind += 2;
				} else if (args[optind].equals("--")) {
					++optind;
					break;
				} else {
					usageError();
					System.exit(2);
				}
			}
		} catch (NumberFormatException e) {
			usageError();
			System.exit(2);
		}

		if (args.length != optind + 1) {
			usageError();
			System.exit(2);
		}

		if (stdinFile != null)
			System.setIn(new FileInputStream(stdinFile));

		String			fileName = args[optind];

		MyMMU			mmu = new MyMMU();
		MyOperatingSystem	os = new MyOperatingSystem();
		MIPSVirtualMachine	mach = new MIPSVirtualMachine(nbrPages * Page.PAGE_SIZE, mmu, os);

		mach.setDebugMode(debugMode);

		if (runVisualizer) {
			Visualizer	visualizer = new Visualizer(mach, nbrPages, Page.PAGE_SIZE);
			mach.setHookProvider(visualizer);
		}
		
		mach.loadExecutable(new FileInputStream(fileName));

		try {
			mach.execute();
		} catch (VMException e) {
			System.err.println("Exception at " + mach.getProgramCounter() + ": " + e);
			System.exit(1);
		}

		System.err.println("Page faults = " + mach.getPageFaultCount());
		System.err.println("Page in count = " + mach.getPageInCount());
		System.err.println("Page out count = " + mach.getPageOutCount());
		System.err.println("Instruction count = " + mach.getInstructionCount());
	}
}
