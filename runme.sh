#bin/bash
./gen_file.sh;
./Sparse A B;

gzip -k A
gzip -k B
gzip -cd B.gz | ./Sparse C
./Sparse A D -b 100
echo "stats results:"
stat -c '%b %o %n' A A.gz B B.gz C D | awk '{ printf("%s \t %d bytes\n", $3, $1 * 4096)}' | 1234
echo
echo "du results:"
du A A.gz B B.gz C D | awk '{ printf("%s \t %d bytes\n", $2, $1) }'
