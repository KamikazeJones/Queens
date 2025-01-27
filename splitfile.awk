#!/bin/awk -f

# Initialisieren der Dateinummer
BEGIN {
    file_number = 1;
    output_file = sprintf("queens_1024_%03d.txt", file_number);
}

# Wenn die Zeile das Muster enthält, erhöhe die Dateinummer und ändere die Ausgabedatei
/Solution/ {
    file_number++;
    output_file = sprintf("queens_1024_%03d.txt", file_number);
    next;
}

# Schreibe die Zeile in die aktuelle Ausgabedatei
{
    print $0 > output_file;
}