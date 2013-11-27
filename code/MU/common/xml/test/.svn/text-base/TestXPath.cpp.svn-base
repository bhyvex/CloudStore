/*
 * =========================================================================
 *
 *       Filename:  TestXPath.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-15 19:25:35
 *  Last Modified:  2012-02-15 22:15:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "xml/Xml.h"

#include <iostream>

int
main(int argc, char *argv[])
{
    Xml *pXml = new Xml();

    xmlNode *pRootNode = pXml->loadFile("TestXPath.xml");

    xmlNode *pNode = pXml->getNodeByXPath("/Root/bookstore/book[2]/title");

    int rt = 0;
    std::string value;

    if (NULL != pNode) {
        pXml->getNodeValue(pNode, &value);
        std::cout << value << std::endl;
    }

    pNode = pXml->getChildNodeByNodeName(pRootNode, "bookstore");

    pNode = pXml->getNodeByXPath(pNode, "book[1]/title");

    if (NULL != pNode) {
        pXml->getNodeValue(pNode, &value);
        std::cout << value << std::endl;
    }

    pNode = pXml->getNodeByXPath(pNode, "/Root/bookstore/book[1]/title");

    if (NULL != pNode) {
        pXml->getNodeValue(pNode, &value);
        std::cout << value << std::endl;
    }

    delete pXml;

    return 0;
}

