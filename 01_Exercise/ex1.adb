-- TK 21Apr2017
-- gnatmake ex1.adb
--
with Ada.Text_IO;         use Ada.Text_IO;
with Ada.Containers;      use Ada.Containers;
with GNAT.String_Split;   use GNAT.String_Split;
with Ada.Strings.Fixed;   use Ada.Strings.Fixed;
with Ada.Numerics.Generic_Elementary_Functions; 
with Ada.Containers.Vectors; 
with Ada.Command_Line;
with Ada.Characters.Latin_1;
with Ada.Exceptions; 
use Ada.Characters;
use GNAT;
use Ada;

procedure ex1 is
   
   type    Real   is digits 16; -- Just change here!
   subtype RealPl is Real range 0.0 .. Real'Last with Dynamic_Predicate => RealPl /= 0.0;
   
   package Value_Container is new Vectors (Natural, RealPl);
   package Real_Functions is new Ada.Numerics.Generic_Elementary_Functions(Real);   
   use Value_Container;
   use Real_Functions;
   
   function geom_mean(x : Vector) return Real is
      sum : Real := 0.0;
   begin
      for e of x loop
         sum := sum + log(e);
      end loop;
      return exp(sum / real(x.length));
   end geom_mean;
  
   capa_extend : constant Count_Type := 1_024 * 1_024;
   file        : File_Type;    
   values      : array ( 1 .. 2) of Vector;
   lineno      : Natural := 0;
   
begin
   if command_line.argument_count < 1 then
      raise Constraint_Error with "Not enough arguments";
   end if;
        
   open(file, in_file, command_line.argument(1));
   
   while not end_of_file(file) loop
      declare
	 fields : String_Split.Slice_Set;
         line   : String  := get_line (file);
	 idx    : Natural := index(line, "#");
	 loc    : Natural range 1 .. 2;
      begin
	 lineno := lineno + 1;
	 
	 -- Remove everything after a comment sign
	 if idx > 0 then
	    line := line(1 .. idx);
	 end if;
	 
	 --  Create the split, using Multiple mode to treat strings of multiple
	 --  whitespace characters as a single separator.
	 --  This populates the Subs object.
	 create(s          => fields,
		from       => line,
		separators => " ;" & Latin_1.HT,
		mode       => string_split.multiple);
	 
	 if slice_count (fields) > 0 then
	    -- This line is not empty
	    if slice_count (fields) /= 3 then
	       raise Constraint_Error with "Wrong number of fields";
	    end if;

	    loc := Natural'value(slice (fields, 2));
		  
	    values(loc).append(RealPl'value(slice (fields, 3)));
	    
	    -- Exend vector capacity if necessary
	    if values(loc).length = values(loc).capacity then
	       values(loc).reserve_capacity(length(values(loc)) + capa_extend);
	    end if;
         end if;

      exception
	 when event : others =>
	    put("Line" & Natural'image(lineno) & " " & ada.exceptions.exception_information (event));	    
      end;

   end loop;
   
   close(file);
   
   put_line("Lines read = " & Natural'image(lineno));

   for i in values'range loop
      put_line (
                "Loc " & Natural'image(i) 
                & " values = " & Count_Type'image(length(values(i))) &
                  " GeoMean = " & Real'image(geom_mean(values(i))));
   end loop;
end ex1;
      

