reset
#this script plots the monitor data from MOTS2
#title & labels 
set title "Objective Space" 
set xlabel "f1" 
set ylabel "f2" 
set grid

# Line styles
set border linewidth 1.5
set style line 1 linecolor rgb '#0000ff'  pointtype -1 pointsize default # blue 
set style line 2 linecolor rgb '#ff0000' linetype 2 linewidth 3  pointtype 0 pointsize default # red 
set style line 3 linecolor rgb '#00ff00' pointtype -1 pointsize default # green 
set style line 4 linecolor rgb '#00ff00' pointtype 2 pointsize 6 # yellow 
set style line 5 linecolor rgb '#ff0000' linetype 1 linewidth 2  pointtype 1 pointsize default # red  
#Legend
set key under
#MOTS2-specific configuration settings
 
 

plot 'ZDT2.txt' title 'ZDT2' with linespoints ls 5,\
	'memories/case_7_datum_point.txt' title 'datum point' with linespoints ls 4,\
	'TS.txt' title 'MOTS2 GPU' pointsize 3,\
	'memories/case_7_HISTORY_objectives_only.txt'  title 'search history' 

#EPS 
#set terminal postscript eps size 3.5,2.62 enhanced color \
#    font 'Helvetica,14' linewidth 2 
#set output 'MOTS2_monitor_data.eps'
#replot 
#set term pop
#set output 
