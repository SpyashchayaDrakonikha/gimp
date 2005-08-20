<?xml version="1.0" encoding="UTF-8"?>

<!--  simple XSL transformation to create a header file from authors.xml  -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:dc="http://purl.org/dc/elements/1.1/">

  <xsl:output method="text" />
  <xsl:template match="/dc:gimp-authors">
<xsl:text>
/* NOTE: This file is autogenerated from authors.xml, do not edit it. */

static const gchar *authors[] =
{
</xsl:text>
  <xsl:apply-templates select="dc:creator" />
  <xsl:apply-templates select="dc:contributor" />
<xsl:text>  NULL
};
</xsl:text>
  </xsl:template>

  <xsl:template match="dc:creator">  "<xsl:apply-templates />",
</xsl:template>
  <xsl:template match="dc:contributor">  "<xsl:apply-templates />",
</xsl:template>

</xsl:stylesheet>
