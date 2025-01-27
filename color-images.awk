#!/bin/awk -f

function c() {
	c1 = int( rand() *256 );
	c2 = int( rand() *256 );
	c3 = int( rand() *256 );
	return c1 " " c2 " " c3;
}

BEGIN {
	srand();

	print "#!/bin/bash"
	print

	for(i=1; i<=100; i++) {
		filename = sprintf("queens_1024_%03d.txt", i);
		cmd = "./restart.sh better-gcc-Queens 1024 " filename " 20"
		print cmd
	}

	for(i=1; i<=100; i++) {
		filename = sprintf("queens_1024_%03d.txt", i);
		ppmfile = sprintf("queens_1024_%03d.ppm", i);
		pngfile = sprintf("queens_1024_%03d.png", i);
                
		cmd = "./makeppmpic.awk background=\"" c() "\" queen=\"" c() "\" color1=\"" c() "\" color2=\"" c() "\" color=\"" c() "\" pixsize=1 pixcount=1024 " filename " > " ppmfile;
		print cmd;
		cmd = "convert " ppmfile " " pngfile;
		print cmd;
	}
}


