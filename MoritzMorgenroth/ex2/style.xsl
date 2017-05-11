<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" >
<xsl:output method="text" omit-xml-declaration="yes" indent="no"/>
<xsl:template match="gasDay[@boundaryNode[@date, time[@hour, @minute ,amountOfPower[@value]]]">
     <element>
       <xsl:value-of select=
        "concat(@date, ';', @hour, @minute)"/>
     </element>
 </xsl:template>
</xsl:stylesheet>