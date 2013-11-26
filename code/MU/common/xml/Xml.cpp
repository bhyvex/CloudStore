/*
 * =========================================================================
 *
 *       Filename:  Xml.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-15 17:54:14
 *  Last Modified:  2012-02-15 17:55:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "Xml.h"

#include <string>
#include <list>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

#include "log/log.h"

#define XML_VERSION             "1.0"
#define XML_ENCODING            "UTF-8"
#define XML_INDENT_LEVEL        0
#define XML_FORMAT_ALLOWED      1

Xml::Xml()
{
    m_pDoc = NULL;
}

Xml::~Xml()
{
    xmlFreeDoc(m_pDoc);
    m_pDoc = NULL;
}

xmlNode *
Xml::loadFile(const std::string &file)
{
    if (NULL != m_pDoc) {
        WARN_LOG("In Xml::loadFile, m_pDoc != NULL");
        xmlFreeDoc(m_pDoc);
        m_pDoc = NULL;
    }

    m_pDoc = xmlReadFile(file.c_str(), XML_ENCODING,
                         int(XML_PARSE_RECOVER));

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::loadFile, xmlReadFile() error");
        return NULL;
    }

    xmlNode *pRootNode = xmlDocGetRootElement(m_pDoc);

    if (NULL == pRootNode) {
        ERROR_LOG("In Xml::loadFile, xmlDocGetRootElement() error");
        return NULL;
    }

    return pRootNode;
}


xmlNode *
Xml::loadMemory(const char *pBuf, int size)
{
    if (NULL == pBuf) {
        FATAL_LOG("In Xml::loadMemory, pBuf == NULL");
        return NULL;
    }

    if (NULL != m_pDoc) {
        WARN_LOG("In Xml::loadFile, m_pDoc != NULL");
        xmlFreeDoc(m_pDoc);
        m_pDoc = NULL;
    }

    m_pDoc = xmlReadMemory(pBuf, size, NULL, XML_ENCODING,
                           int(XML_PARSE_RECOVER));

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::loadMemory, xmlReadMemory() error");
        return NULL;
    }

    xmlNode *pRootNode = xmlDocGetRootElement(m_pDoc);

    if (NULL == pRootNode) {
        ERROR_LOG("In Xml::loadFile, xmlDocGetRootElement() error");
        return NULL;
    }

    return pRootNode;
}


int
Xml::saveToBuffer(xmlNode *pCurNode, std::string *pBuf)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::saveToBuffer, pCurNode == NULL");
        return -1;
    }

    if (NULL == pBuf) {
        FATAL_LOG("In Xml::saveToBuffer, pBuf == NULL");
        return -1;
    }

    xmlBuffer *pXmlBuf = xmlBufferCreate();

    if (NULL == pXmlBuf) {
        ERROR_LOG("In Xml::saveToBuffer, xmlBufferCreate() error");
        return -1;
    }

    int bytes = xmlNodeDump(pXmlBuf, m_pDoc, pCurNode,
                            XML_INDENT_LEVEL, XML_FORMAT_ALLOWED);

    if (bytes < 0) {
        ERROR_LOG("In Xml::saveToBuffer, xmlNodeDump() error");
        return -1;
    }

    *pBuf = (const char *)(pXmlBuf->content);
    xmlBufferFree(pXmlBuf);
    pXmlBuf = NULL;

    return 0;
}

int
Xml::saveToBuffer(std::string *pBuf)
{
    if (NULL == pBuf) {
        FATAL_LOG("In Xml::saveToBuffer, pBuf == NULL");
        return -1;
    }

    xmlNode *pRootNode = xmlDocGetRootElement(m_pDoc);

    if (NULL == pRootNode) {
        ERROR_LOG("In Xml::saveToBuffer, xmlDocGetRootElement() error");
        return -1;
    }

    return saveToBuffer(pRootNode, pBuf);
}

int
Xml::saveToFile(const std::string &file)
{
    int nwrite = xmlSaveFormatFileEnc(file.c_str(), m_pDoc,
                                      XML_ENCODING, XML_FORMAT_ALLOWED);

    if (-1 == nwrite) {
        ERROR_LOG("In Xml::saveToFile, xmlSaveFormatFileEnc() error");
        return -1;
    }

    return 0;
}

xmlNode *
Xml::createDocument(const std::string &rootName)
{
    if (NULL != m_pDoc) {
        WARN_LOG("In Xml::createDocument, m_pDoc != NULL");
        xmlFreeDoc(m_pDoc);
        m_pDoc = NULL;
    }

    m_pDoc = xmlNewDoc(BAD_CAST XML_VERSION);

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::createDocument, xmlNewDoc() error");
        return NULL;
    }

    xmlNode *pRootNode = xmlNewNode(NULL, BAD_CAST rootName.c_str());
    xmlDocSetRootElement(m_pDoc, pRootNode);

    return pRootNode;
}

xmlNode *
Xml::getRootNode() const
{
    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getRootNode, m_pDoc == NULL");
        return NULL;
    }

    return xmlDocGetRootElement(m_pDoc);
}


xmlNode *
Xml::getParentNode(xmlNode *pCurNode) const
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getParentNode, pCurNode == NULL");
        return NULL;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getParentNode, m_pDoc == NULL");
        return NULL;
    }

    return pCurNode->parent;
}

xmlNode *
Xml::getChildNode(xmlNode *pCurNode) const
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getFirstChildNode, pCurNode == NULL");
        return NULL;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getFirstChildNode, m_pDoc == NULL");
        return NULL;
    }

    return pCurNode->children;
}

int
Xml::getChildNodes(xmlNode *pCurNode, std::list<xmlNode *> *pNodeList)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getChildNodes, pCurNode == NULL");
        return -1;
    }

    if (NULL == pNodeList) {
        FATAL_LOG("In Xml::getChildNodes, pNodeList == NULL");
        return -1;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getChildNodes, m_pDoc == NULL");
        return -1;
    }

    xmlNode *pChildNode = pCurNode->children;

    while (NULL != pChildNode) {
        pNodeList->push_back(pChildNode);
        pChildNode = pChildNode->next;
    }

    return 0;
}

xmlNode *
Xml::getChildNodeByNodeName(xmlNode *pCurNode,
                            const std::string &nodeName)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getChildNodeByNodeName, pCurNode == NULL");
        return NULL;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getChildNodeByNodeName, m_pDoc == NULL");
        return NULL;
    }

    xmlNode *pChildNode = pCurNode->children;

    while (NULL != pChildNode) {
        if (1 == xmlStrEqual(pChildNode->name,
                             BAD_CAST nodeName.c_str())) {
            return pChildNode;
        }

        pChildNode = pChildNode->next;
    }

    return NULL;
}


int
Xml::getChildNodesByNodeName(xmlNode *pCurNode,
                             const std::string &nodeName,
                             std::list<xmlNode *> *pNodeList)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getChildNodesByNodeName, pCurNode == NULL");
        return -1;
    }

    if (NULL == pNodeList) {
        FATAL_LOG("In Xml::getChildNodesByNodeName, pNodeList == NULL");
        return -1;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getChildNodesByNodeName, m_pDoc == NULL");
        return -1;
    }

    xmlNode *pChildNode = pCurNode->children;

    while (NULL != pChildNode) {
        if (1 == xmlStrEqual(pChildNode->name,
                             BAD_CAST nodeName.c_str())) {
            pNodeList->push_back(pChildNode);
        }

        pChildNode = pChildNode->next;
    }

    return 0;
}


int
Xml::getChildNodeValueByNodeName(xmlNode *pCurNode,
                                 const std::string &childNodeName,
                                 std::string *pValue)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getChildNodeValueByNodeName, pCurNode == NULL");
        return -1;
    }

    if (NULL == pValue) {
        FATAL_LOG("In Xml::getChildNodeValueByNodeName, pValue == NULL");
        return -1;
    }

    xmlNode *pChildNode = getChildNodeByNodeName(pCurNode, childNodeName);

    if (NULL == pChildNode) {
        ERROR_LOG("In Xml::getChildNodeValueByNodeName, "
                  "getChildNodeByNodeName() error");
        return -1;
    }

    return getNodeValue(pChildNode, pValue);
}

int
Xml::getNodeValue(xmlNode *pCurNode, std::string *pValue)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::getNodeValue, pCurNode == NULL");
        return -1;
    }

    if (NULL == pValue) {
        FATAL_LOG("In Xml::getNodeValue, pValue == NULL");
        return -1;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getNodeValue, m_pDoc == NULL");
        return -1;
    }

    xmlChar *pXmlValue = xmlNodeGetContent(pCurNode);

    if (NULL == pXmlValue) {
        ERROR_LOG("In Xml::getNodeValue, xmlNodeGetContent() error");
        return -1;
    }

    *pValue = (const char *) pXmlValue;
    xmlFree(pXmlValue);

    return 0;
}

int
Xml::setNodeValue(xmlNode *pCurNode, const std::string &value)
{
    if (NULL == pCurNode) {
        FATAL_LOG("In Xml::setNodeValue, pCurNode == NULL");
        return -1;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::setNodeValue, m_pDoc == NULL");
        return -1;
    }

    xmlNodeSetContent(pCurNode, BAD_CAST value.c_str());

    return 0;
}

xmlNode *
Xml::addChildNode(xmlNode *pParentNode,
                  const std::string &nodeName,
                  const std::string &nodeValue)
{
    if (NULL == pParentNode) {
        FATAL_LOG("In Xml::addChildNode, pParentNode = NULL");
        return NULL;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::addChildNode, m_pDoc == NULL");
        return NULL;
    }

    return xmlNewTextChild(pParentNode, NULL,
                           BAD_CAST nodeName.c_str(),
                           BAD_CAST nodeValue.c_str());
}

xmlNode *
Xml::addChildNode(xmlNode *pParentNode,
                  const std::string &nodeName)
{
    if (NULL == pParentNode) {
        FATAL_LOG("In Xml::addChildNode, pParentNode == NULL");
        return NULL;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::addChildNode, m_pDoc = NULL");
        return NULL;
    }

    xmlNode *pChildNode = xmlNewNode(NULL, BAD_CAST nodeName.c_str());

    if (NULL == pChildNode) {
        ERROR_LOG("In Xml::addChildNode, xmlNewNode() error");
        return NULL;
    }

    return xmlAddChild(pParentNode, pChildNode);
}

xmlNode *
Xml::addChildNode(xmlNode *pParentNode, xmlNode *pChildNode)
{
    if (NULL == pParentNode) {
        FATAL_LOG("In Xml::addChildNode, pParentNode == NULL");
        return NULL;
    }

    if (NULL == pChildNode) {
        FATAL_LOG("In Xml::addChildNode, pChildNode == NULL");
        return NULL;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::addChildNode, m_pDoc == NULL");
        return NULL;
    }

    return xmlAddChild(pParentNode, pChildNode);
}


xmlNode *
Xml::getNodeByXPath(xmlNode *pCurNode, const std::string &xpath)
{
    // 允许pCurNode == NULL
    //if (NULL == pCurNode) {
    //FATAL_LOG("In Xml::getNodeByXPath, pCurNode == NULL");
    //return NULL;
    //}

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getNodeByXPath, m_pDoc == NULL");
        return NULL;
    }

    // 初始化XPath上下文
    xmlXPathContext *pContext = xmlXPathNewContext(m_pDoc);

    if (NULL == pContext) {
        ERROR_LOG("In Xml::getNodeByXPath, xmlXPathNewContext() error");
        return NULL;
    }

    // 设置当前节点
    if (NULL != pCurNode) {
        pContext->node = pCurNode;
    }

    // 解析xpath表达式
    xmlXPathObject *pResult = xmlXPathEvalExpression(
                                  BAD_CAST xpath.c_str(), pContext);
    // 释放XPath上下文对象
    xmlXPathFreeContext(pContext);
    pContext = NULL;

    if (NULL == pResult) {
        ERROR_LOG("In Xml::getNodeByXPath, "
                  "xmlXPathEvalExpression() error");
        return NULL;
    }

    // 解析表达式结果集
    xmlNode *pNode = NULL;
    xmlNodeSet *pNodeSet = pResult->nodesetval;

    if (NULL == pNodeSet) {
        ERROR_LOG("In Xml::getNodeByXPath, pNodeSet == NULL");
        xmlXPathFreeObject(pResult);

        return NULL;
    }

    if (pNodeSet->nodeNr >= 1) {
        pNode = pNodeSet->nodeTab[0];

    } else {
        ERROR_LOG("In Xml::getNodeByXPath, no path '%s' found",
                  xpath.c_str());
    }

    // 释放XPath表达式计算结果对象
    xmlXPathFreeObject(pResult);
    pResult = NULL;

    return pNode;
}

xmlNode *
Xml::getNodeByXPath(const std::string &xpath)
{
    return getNodeByXPath(NULL, xpath);
}

int
Xml::getNodesByXPath(xmlNode *pCurNode, const std::string &xpath,
                     std::list<xmlNode *> *pNodeList)
{
    // 允许pCurNode == NULL

    if (NULL == pNodeList) {
        FATAL_LOG("In Xml::getNodesByXPath, pNodeList == NULL");
        return -1;
    }

    if (NULL == m_pDoc) {
        ERROR_LOG("In Xml::getNodesByXPath, m_pDoc == NULL");
        return -1;
    }

    // 初始化XPath上下文
    xmlXPathContext *pContext = xmlXPathNewContext(m_pDoc);

    if (NULL == pContext) {
        ERROR_LOG("In Xml::getNodeByXPath, xmlXPathNewContext() error");
        return NULL;
    }

    // 设置当前节点
    if (NULL != pCurNode) {
        pContext->node = pCurNode;
    }

    // 解析xpath表达式
    xmlXPathObject *pResult = xmlXPathEvalExpression(
                                  BAD_CAST xpath.c_str(), pContext);
    // 释放XPath上下文对象
    xmlXPathFreeContext(pContext);
    pContext = NULL;

    if (NULL == pResult) {
        ERROR_LOG("In Xml::getNodeByXPath, "
                  "xmlXPathEvalExpression() error");
        return NULL;
    }

    // 解析表达式结果集
    xmlNode *pNode = NULL;
    xmlNodeSet *pNodeSet = pResult->nodesetval;

    if (NULL == pNodeSet) {
        ERROR_LOG("In Xml::getNodeByXPath, pNodeSet == NULL");
        xmlXPathFreeObject(pResult);

        return NULL;
    }

    for (int i = 0; i < pNodeSet->nodeNr; ++i) {
        pNodeList->push_back(pNodeSet->nodeTab[i]);
    }

    xmlXPathFreeObject(pResult);

    return 0;
}

int
Xml::getNodesByXPath(const std::string &xpath,
                     std::list<xmlNode *> *pNodeList)
{
    return getNodesByXPath(NULL, xpath, pNodeList);
}

int
Xml::getNodeValueByXPath(const std::string &xpath, std::string *pValue)
{
    if (NULL == pValue) {
        FATAL_LOG("In Xml::getNodeValueByXPath, pValue == NULL");
        return -1;
    }

    xmlNode *pNode = getNodeByXPath(xpath);

    if (NULL == pNode) {
        ERROR_LOG("In Xml::getNodeValueByXPath, getNodeByXPath() error");
        return -1;
    }

    return getNodeValue(pNode, pValue);
}

int
Xml::getNodeValueByXPath(xmlNode *pCurNode, const std::string &xpath,
                         std::string *pValue)
{
    // 允许pCurNode == NULL
    //if (NULL == pCurNode) {
    //FATAL_LOG("In Xml::getNodeValueByXPath, pCurNode == NULL");
    //return -1;
    //}

    if (NULL == pValue) {
        FATAL_LOG("In Xml::getNodeValueByXPath, pValue == NULL");
        return -1;
    }

    xmlNode *pNode = getNodeByXPath(pCurNode, xpath);

    if (NULL == pNode) {
        ERROR_LOG("In Xml::getNodeValueByXPath, getNodeByXPath() error");
        return -1;
    }

    return getNodeValue(pNode, pValue);
}

