public class ex3 {
public static void selfWrite(String[] s){
System.out.println(String.join("\n", s));
for(int i=0; i<s.length; i++){
s[i] = s[i].replaceAll("\\\\","\\\\\\\\");
s[i] = s[i].replaceAll("\\\"","\\\\\\\"");
}
System.out.println("selfWrite(new String[]{\""+String.join("\",\n\"",s)+"\"});\n}\n}");
}
public static void main(String[] args){
selfWrite(new String[]{"public class ex3 {",
"public static void selfWrite(String s){",
"System.out.println(String.join(\"\\n\", s));",
"for(int i=0; i<s.length; i++){",
"s[i] = s[i].replaceAll(\"\\\\\\\\\",\"\\\\\\\\\\\\\\\\\");",
"s[i] = s[i].replaceAll(\"\\\\\\\"\",\"\\\\\\\\\\\\\\\"\");",
"}",
"System.out.println(\"selfWrite(new String[]{\\\"\"+String.join(\"\\\",\\n\\\"\",s)+\"\\\"});\\n}\\n}\");",
"}",
"public static void main(String[] args){"});
}
}