import mmap
import os
import subprocess

def dump_physical_memory(filename):
    """Dumps the physical memory to a file.

    Args:
        filename: The name of the file to write the memory dump to.
    """

    try:
        # Check SELinux status and temporarily disable it if necessary
        selinux_status = subprocess.check_output(["getenforce"])
        if selinux_status.decode().strip() == "Enforcing":
            print("Disabling SELinux temporarily...")
            subprocess.check_output(["setenforce", "0"])

        # Load the mem module if not already loaded
        modules = subprocess.check_output(["lsmod"])
        if "mem" not in modules.decode():
            print("Loading mem module...")
            subprocess.check_output(["modprobe", "mem"])

        # Open /dev/mem for reading
        mem_file = os.open("/dev/mem", os.O_RDONLY)

        # Get the system's physical memory size
        memory_size = os.sysconf("SC_PHYS_PAGES") * os.sysconf("SC_PAGESIZE")

        # Create a memory-mapped file for the dump
        with open(filename, "wb") as dump_file:
            with mmap.mmap(mem_file, memory_size) as mapped_mem:
                # Write the memory contents to the file
                dump_file.write(mapped_mem)

    except Exception as e:
        print(f"Error dumping physical memory: {e}")

if __name__ == "__main__":
    dump_physical_memory("memory_dump.bin")
