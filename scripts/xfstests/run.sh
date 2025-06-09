#!/bin/bash
export FSTYP=(gfs2 or ocfs2)
export TEST_DEV=/path/to/dev
export TEST_DIR=/path/to/mnt
unset SCRATCH_DEV
unset SCRATCH_MNT

TESTS="
generic/001 generic/002 generic/005 generic/006 generic/007
generic/011 generic/013 generic/014 generic/023 generic/028
generic/035 generic/075 generic/080 generic/086 generic/087
generic/088 generic/091 generic/112 generic/113 generic/123
generic/126 generic/131 generic/184 generic/193 generic/198
generic/207 generic/210 generic/211 generic/212 generic/213
generic/214 generic/215 generic/221 generic/228 generic/236
generic/240 generic/245 generic/246 generic/247 generic/248
generic/249 generic/257 generic/258 generic/263 generic/286
generic/308 generic/309 generic/313 generic/314 generic/355
generic/360 generic/378 generic/391 generic/394 generic/420
generic/423 generic/426 generic/428 generic/430 generic/431
generic/432 generic/433 generic/434 generic/436 generic/437
generic/443 generic/445 generic/448 generic/450 generic/451
generic/465 generic/467 generic/469 generic/477 generic/478
"

./check $TESTS

