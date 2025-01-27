#!/bin/awk -f

function repeat(char, n, result, i) {
    result = ""
    for (i = 0; i < n; i++) {
        result = result char
    }
    return result
}

BEGIN {
    # Standardwerte setzen
    pixsize = 10;
    pixcount = 50;
    colorA = "0 0 0"; # Schwarz
    colorB = "255 0 0"; # Rot
    colorC = "173 216 230"; # Hellblau
    colorD = "0 0 255"; # Blau
    colorE = "0 0 139"; # Dunkelblau

    height = 0;
    # Übergebene Parameter verarbeiten
    for (i = 1; i < ARGC; i++) {
        if (ARGV[i] ~ /^pixsize=/) {
            split(ARGV[i], a, "=");
            pixsize = a[2];
        } else if (ARGV[i] ~ /^pixcount=/) {
            split(ARGV[i], a, "=");
            pixcount = a[2];
        } else if (ARGV[i] ~ /^background=/) {
            split(ARGV[i], a, "=");
            colorA = a[2];
        } else if (ARGV[i] ~ /^queen=/) {
            split(ARGV[i], a, "=");
            colorB = a[2];
        } else if (ARGV[i] ~ /^color1=/) {
            split(ARGV[i], a, "=");
            colorC = a[2];
        } else if (ARGV[i] ~ /^color2=/) {
            split(ARGV[i], a, "=");
            colorD = a[2];
        } else if (ARGV[i] ~ /^color3=/) {
            split(ARGV[i], a, "=");
            colorE = a[2];
        } else if (ARGV[i] ~ /^height=/) {
            split(ARGV[i], a, "=");
            height = a[2];        
        }
    }
    
	width = pixsize * pixcount;
	if(height == 0) {
        height = width;
    }

    # PPM Header
    print "P3";
    print width, height;
    print "255";

    r = 0;
    rA = repeat("A ", pixsize);
    rB = repeat("B ", pixsize);
    rC = repeat("C ", pixsize);
    rD = repeat("D ", pixsize);
    rE = repeat("E ", pixsize);
}

/^[ .123]*#[ .123]*$/ {
	gsub(/\./,rA,$0)
	gsub(/#/,rB,$0)
	gsub(/1/,rC,$0)
	gsub(/2/,rD,$0)
	gsub(/3/,rE,$0)

	gsub(/A/, colorA, $0);
	gsub(/B/, colorB, $0);
    gsub(/C/, colorC, $0);
    gsub(/D/, colorD, $0);
    gsub(/E/, colorE, $0);
    
    for(i=0; i<pixsize; i++) {
        print $0
    }
}