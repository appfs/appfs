# TK 28Apr2017
# APPFS Exercise 1: Read CSV file
#
BEGIN { FS = ";" }
# Remove comments and skip empty lines 
/^#/ || NF == 0 { next; } 
# Process good lines
NF == 3 && /^[ ]*[0-9]+[ ]*;[ ]*[12][ ]*;[ ]*[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$/ {
    loc = $2 + 0;
    val = $3 + 0.0;
    
    if (val > 0.0) {
       value[loc, count[loc]++] = val;
       next;
    }
}
# Error otherwise
{ print FNR, "error", $0 }
# print result
END {
   print FILENAME, ": Read ", FNR, "lines";
   for(k in count) {
         sum = 0.0;
         for(i = 0; i < count[k]; i++) {
            sum += log(value[k,i]);
         }
         print "Loc", k, count[k], exp(sum / count[k]);
   }
}
