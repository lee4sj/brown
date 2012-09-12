package vm;

import vmsupport.*;

public class MyMMU implements MMU {
	/*
	 * The following static variable 'policy' will be set to one of the following values:
	 *  ReplacementPolicy.LRU
	 *  ReplacementPolicy.LFU
	 *  ReplacementPolicy.Random
	 * Use it in this class and MyOperatingSystem to determine which page replacement
	 * policy to use.
	 */
	public static ReplacementPolicy		policy = ReplacementPolicy.LRU;

	/*
	 * You must implement the following function, which is called on every memory access.
	 * It should translate the given virtual address into a physical address and return it.
	 * If the page containing the requested virtual address is not in physical memory, 
	 * throw a new PageFault exception.  You should also update the page table as is appropriate.
	 *
	 * Arguments:
	 *  mach - an instance of the MIPSVirtualMachine that is executing the program
	 *  virtualAddress - the virtual address of the memory access
	 *  isStore - true if the memory access is a store operation, false if it's a load
	 *
	 * Returns:
	 *  The physical address corresponding to the virtual address
	 *
	 * Throws:
	 *  PageFault exception, if the virtual address is not in physical memory
	 *
	 * NB: Memory access and virtual-to-physical translation is normally done in HARDWARE.
	 *     However, it's a conceptually simple task, so we're having you implement it in Java code.
	 *     Consequentially, you MUST follow the rules below.
	 *
	 * Rules:
	 *  1. You cannot add any variables or methods to this class
	 *  2. You may access ONLY mach, virtualAddress, isStore, and policy
	 *  3. You cannot use any other Java class
	 *  4. You cannot use iteration (e.g. while and for) or recursion
	 *
	 * Finally, you should use mach.getPageSize() to determine the page size.  Don't hard code
	 * the page size!
	 */
	public long	accessMemory (MIPSVirtualMachine mach, long virtualAddress, boolean isStore) throws VMException
	{
		long vpn = virtualAddress / mach.getPageSize();			//virtual page number
		long offset = virtualAddress % mach.getPageSize();		//offset
		PageTable table = mach.getPageTable();					//page table

		if(!table.getBit0(vpn))									//if the page is not loaded in the physical memory
			throw new PageFault(virtualAddress);


		//by this point, assume that the page is loaded in the physical memory.

		if(isStore)												//if the instruction requires storing data
			table.setBit1(vpn, true);							// set the memory as dirty

		//update LRU or LFU data
		if(MyMMU.policy == ReplacementPolicy.LRU)				//if LRU
			table.setWord1(vpn, mach.getInstructionCount());	// set Word1 as the current instruction position
		if(MyMMU.policy == ReplacementPolicy.LFU)				//if LFU
			table.setWord1(vpn, table.getWord1(vpn)+1);			// increase Word1 by one

		return (table.getWord0(vpn) + offset);

	}

}
































