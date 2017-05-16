<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" >
<xsl:output method="text" indent="no"/>
<xsl:strip-space elements="*"/>
<xsl:template match="measured/gasDay">
<xsl:for-each select="boundaryNode/time/amountOfPower">
<xsl:value-of select="ancestor::gasDay[1]/@date"/>
<xsl:text>; </xsl:text>
<xsl:if test="string-length(ancestor::time[1]/@hour) &lt; 2">
<xsl:text>0</xsl:text>
</xsl:if>
<xsl:value-of select="ancestor::time[1]/@hour"/>
<xsl:text>; </xsl:text>
<xsl:value-of select="@value"/>
<xsl:text>&#10;</xsl:text>
</xsl:for-each>
</xsl:template>
</xsl:stylesheet>