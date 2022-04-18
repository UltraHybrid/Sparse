 #bin/bash
# 4194302
dd if=/dev/zero of=tmp bs=4194302 count=1;
sed -e 's/.\{0\}/&1/; s/.\{9999\}/&1/; s/$/1/' tmp > A;
rm tmp;
