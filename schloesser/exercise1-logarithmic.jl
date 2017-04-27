filename = ARGS[1]

f = open(filename)
valid = [0,0,0]
mean = [0.0,0.0]
for (index, line) in enumerate(eachline(f))
    valid[3] = valid[3]+1
	tmp = split( split( split( line, "\n")[1], "#")[1], "; ")
    if length(tmp) < 3
        continue
    end 
    try
        pos = parse(Int, tmp[2])
        val = float(tmp[3])
        if val>0 && (pos == 1 || pos == 2)
            mean[pos] = mean[pos] + log(val)
            valid[pos] = valid[pos]+1
        end
    catch err
        continue
    end
end

println("File: " * string(filename) * " with " * string(valid[3]) * " lines")
for i = 1:2
    mean[i] = exp(mean[i]/valid[i])
    println("Valid values Loc" * string(i) * ": " * string(valid[i]) * " with GeoMean: " * string(mean[i]))
end
