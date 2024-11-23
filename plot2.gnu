# I didn't do this, I'm just experimenting for the first time in gnuplot

set terminal qt size 1280,720 enhanced font "Arial,10"

set datafile binary format="%float" array=(1150,648)
set xrange [0:1150]
set yrange [648:0]  
set view map
set palette gray  
unset colorbox  

set cbrange [-1.0e-3:1.0e-3] 

filepath = "data/snapshots/"
filelist = system("ls " . filepath . "*.bin")  
files = words(filelist) 

print "Press 'q' at any time to stop the animation."

do for [i=1:files] {
    filename = word(filelist, i)
    print sprintf("Plotting %s...", filename)

    plot filename binary array=(1150,648) with image title sprintf("Snapshot: %s", filename)

    pause 0.3 "Press 'q' to quit or wait for the next frame"
}

