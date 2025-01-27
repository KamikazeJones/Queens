#!/bin/awk -f

BEGIN {
    # Standardwerte setzen
    pixsize = 10;
    pixwidth = 50;
    background = "darkgrey";
    colorA = "black";
    colorB = "red";
    colorC = "lightblue";
    colorD = "blue";
    colorE = "cyan";
    perline = 1;
    lines = 1;
    marginx = 20;
    marginy = 20;
    pixheight = pixwidth;

    # Übergebene Parameter verarbeiten
    for (i = 1; i < ARGC; i++) {
        if (ARGV[i] ~ /^pixsize=/) {
            split(ARGV[i], a, "=");
            pixsize = a[2];
        } else if (ARGV[i] ~ /^pixwidth=/) {
            split(ARGV[i], a, "=");
            pixwidth = a[2];
        } else if (ARGV[i] ~ /^background=/) {
            split(ARGV[i], a, "=");
            background = a[2];
        } else if (ARGV[i] ~ /^queen=/) {
            split(ARGV[i], a, "=");
            colorB = a[2];
        } else if (ARGV[i] ~ /^color0=/) {
            split(ARGV[i], a, "=");
            colorA = a[2];
        } else if (ARGV[i] ~ /^color1=/) {
            split(ARGV[i], a, "=");
            colorC = a[2];
        } else if (ARGV[i] ~ /^color2=/) {
            split(ARGV[i], a, "=");
            colorD = a[2];
        } else if (ARGV[i] ~ /^color3=/) {
            split(ARGV[i], a, "=");
            colorE = a[2];
        } else if (ARGV[i] ~ /^perline=/) {
            split(ARGV[i], a, "=");
            perline = a[2];
        } else if (ARGV[i] ~ /^lines=/) {
            split(ARGV[i], a, "=");
            lines = a[2];
        }  else if (ARGV[i] ~ /^pixheight=/) {
            split(ARGV[i], a, "=");
            pixheight = a[2];
        }  else if (ARGV[i] ~ /^marginx=/) {
            split(ARGV[i], a, "=");
            marginx = a[2];
        }  else if (ARGV[i] ~ /^marginy=/) {
            split(ARGV[i], a, "=");
            marginy = a[2];
        }

    }

	width = ( pixsize * pixwidth) * perline + (perline +1) * marginx;
	height = ( pixsize * pixheight) * lines + (lines +1) * marginy;

    print "<svg width=\"" width "\" height=\"" height "\" xmlns=\"http://www.w3.org/2000/svg\">";
    print "<rect x=\"0\" y=\"0\" width=\"" width "\" height=\"" height "\" fill=\"" background "\" />";
    lineno = 0;
}
/^[ .123]*#[ .123]*$/ {
	gsub(/\./,"A",$0)
	gsub(/#/,"B",$0)
	gsub(/1/,"C",$0)
	gsub(/2/,"D",$0)
	gsub(/3/,"E",$0)

	gsub(/A/, "<path d=\"P l" pixsize ",0 l0," pixsize " l-" pixsize ",0 z\" fill=\"" colorA "\" />", $0);
	gsub(/B/, "<path d=\"P l" pixsize ",0 l0," pixsize " l-" pixsize ",0 z\" fill=\"" colorB "\" />", $0);
	gsub(/C/, "<path d=\"P l" pixsize ",0 l0," pixsize " l-" pixsize ",0 z\" fill=\"" colorC "\" />", $0);
	gsub(/D/, "<path d=\"P l" pixsize ",0 l0," pixsize " l-" pixsize ",0 z\" fill=\"" colorD "\" />", $0);
	gsub(/E/, "<path d=\"P l" pixsize ",0 l0," pixsize " l-" pixsize ",0 z\" fill=\"" colorE "\" />", $0);
	
    imageno = int(lineno / pixheight);
    col = imageno % perline;
    row = int(imageno / perline);
    x = 0;
    y = lineno % pixheight;
    imagewidth = pixsize * pixwidth;
    imageheight = pixsize * pixheight;

    print width, height, lineno, imageno, row, col > "/dev/stderr";

    while (match($0, /P/)) {
        sub(/P/, "M" marginx + col*(imagewidth + marginx) + x*pixsize "," marginy + row*(imageheight + marginy) + y*pixsize, $0);
        x++;
    }
    print
    lineno = lineno+1;
}

END {print "</svg>"}
