package vm;

import vmsupport.*;
import java.util.Random;

public class MyOperatingSystem implements OperatingSystem {


	long[] virPageNumber;				// array of virtual page numbers indexed by physical page numbers.
										// physical page number = physical address / page size.
										// if the physical memory at the address do not hold a valid page, -1 will be stored.



	/*
	 * You must implement the following function, which is called when a page fault occurs.
	 * By the time you return, a valid entry for the given virtual address MUST be in the page table.
	 *
	 * You must do the following:
	 *  - Choose a location to place the page in physical memory
	 *    (If there's an unused page in physical memory, you should put it there.  If there is no
	 *     unused page available, you'll have to choose a page to evict)
	 *  - If you evict a page, handle evicting it
	 *  - Page in the requested page, and put an entry for it in the page table
	 *
	 * Arguments:
	 *  mach - an instance of the MIPSVirtualMachine that is executing the program
	 *  virtualAddress - the virtual address of the memory access which caused the page fault
	 *
	 * Since this is normally implemented in software, by the operating system,
	 * you may use auxiliary data structures and other methods to help you implement it.
	 * However, you should strive for the simplest and most efficient solution.
	 */
	public void	handlePageFault (MIPSVirtualMachine mach, long virtualAddress) throws VMException
	{
		PageTable table = mach.getPageTable();
		
		boolean found = false;
		int index = 0;

		long tracker = table.getWord1(virPageNumber[0]);
		int toUse = 0;


		//*** choose a location to place the page in the physical memory
		while(index < virPageNumber.length && !found){
			if(virPageNumber[index] == -1){									// if some physical memory page is unused.
				toUse = index;												//	the current index is the page number
				found = true;
			}

			else if (table.getWord1(virPageNumber[index]) < tracker ){		// otherwise, keep track of the smallest LRU or LFU value
				tracker = table.getWord1(virPageNumber[index]);				
				toUse = index;
			}

			index++;
		}
		
		//If all physical memory is in use and random policy is used
		if(!found && MyMMU.policy == ReplacementPolicy.Random){
			Random random = new Random();
			toUse = random.nextInt(virPageNumber.length);
		}

		long phyPageNum = toUse * mach.getPageSize();
		long vpn = virtualAddress/mach.getPageSize();
		long evictingVpn = virPageNumber[toUse];

		//*** Handle evicting
		if(!found){															// if a page needs to be evicted
			if(table.getBit1(evictingVpn))									// if the page is dirty
				if(!mach.pageOut(evictingVpn, phyPageNum))					//	write the page back into the disk
					throw new SegmentationFault(virtualAddress);			//		if pageOut fails, throw seg.fault.

			table.setBit1(evictingVpn, false);								// set this page to not dirty
		}

		table.setBit0(evictingVpn, false);									// invalidate the evictedVpn in the page table

		//*** Page In
		if(!mach.pageIn(vpn, phyPageNum))									// bring the page from the disk into the physical memory
			throw new SegmentationFault(virtualAddress);					//		if pageIn fails, throw seg.fault!

		//*** update page table
		table.setBit0(vpn, true);											// validate the vpn in the page table
		table.setBit1(vpn, false);											// set the dirty bit to false
		table.setWord0(vpn, phyPageNum);									// set the physical page number of the vpn

		// the LRU or LFU information is left as before
		/*if(MyMMU.policy == ReplacementPolicy.LFU)
			table.setWord1(vpn, 0);											// set the LFU info to default
		if(MyMMU.policy == ReplacementPolicy.LRU)
			table.setWord1(vpn, mach.getInstructionCount());				// set the LRU info to default(current)
		*/
		
		virPageNumber[toUse] = vpn;											// update virPageNumber array

	}

	/*
	 * You must implement the following function, which is called once when the program starts executing.
	 * In this function, you should initialize the page table, and set up any auxiliary data structures
	 * that your virtual operating system uses.
	 *
	 * Arguments:
	 *  mach - an instance of the MIPSVirtualMachine that is executing the program
	 */
	public void	startup (MIPSVirtualMachine mach) throws VMException
	{
		// Initialize the page table
		// This is equivalent to setting the "page table" pointer with the address
		// of the page table.
		mach.setPageTable(new PageTable());
		virPageNumber = new long[mach.getMemorySize()/mach.getPageSize()];
		// since all the physical pages are unused, assign -1 to all.
		for(int a = 0; a < virPageNumber.length; a++){
			virPageNumber[a] = -1;
		}
	}

	/*
	 * You must implement the following function, which is called once when the program stops executing.
	 * In this function, you should write any dirty pages back to disk.
	 *
	 * Arguments:
	 *  mach - an instance of the MIPSVirtualMachine that is executing the program
	 */
	public void	shutdown (MIPSVirtualMachine mach) throws VMException
	{
		PageTable table = mach.getPageTable();

		//iterate through all physical pages
		for(int index = 0; index < virPageNumber.length; index++){
			long vpn = virPageNumber[index];
			if(vpn >= 0 && table.getBit1(vpn)){									// if the physical memory page is in use and is dirty
				if(!mach.pageOut(vpn, index * mach.getPageSize()))					//	write the current page into the disk.
					throw new SegmentationFault(vpn);
			}

		}
	}
}

