/*
 * =========================================================================
 *
 *       Filename:  XmlTest.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-15 19:25:35
 *  Last Modified:  2012-02-15 19:25:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "xml/Xml.h"

int
main(int argc, char *argv[])
{
    Xml *pXml = new Xml();

    pXml->createDocument("Root");

    xmlNode *pParentNode = xmlNewNode(NULL, BAD_CAST "Parent");
    xmlNode *pChildNode = xmlNewNode(NULL, BAD_CAST "Child");
    xmlAddChild(pParentNode, pChildNode);

    pXml->addChildNode(pXml->getRootNode(), pParentNode);

    pXml->saveToFile("test.xml");

    delete pXml;

    return 0;
}

