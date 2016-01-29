rm -f image.mfs
cd memfs_files
mfsgen -cvbf ../image.mfs 200 css images svg index.html
cd ..
mb-objcopy -I binary -O elf32-microblazeel -B microblaze --rename-section .data=.foo,alloc,load,readonly,data,contents image.mfs mfsimage.o 
mb-ar r libmfsimage.a mfsimage.o
rm image.mfs mfsimage.o

