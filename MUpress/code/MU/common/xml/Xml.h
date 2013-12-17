/*
 * =========================================================================
 *
 *       Filename:  Xml.h
 *
 *    Description:  libxml2封装
 *
 *        Version:  1.0
 *        Created:  2012-02-15 05:42:14
 *  Last Modified:  2012-02-15 05:42:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef _Xml_H_
#define _Xml_H_

#include <string>
#include <list>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

class Xml
{
public:
    Xml();

    virtual ~Xml();

    /**
     * @brief 从指定文件中加载xml
     *
     * @param file
     *
     * @return
     */
    xmlNode *loadFile(const std::string &file);

    /**
     * @brief 从指定内存区域加载xml
     *
     * @param pBuf
     * @param size
     *
     * @return
     */
    xmlNode *loadMemory(const char *pBuf, int size);

    /**
     * @brief 将当前节点的所有子孙节点dump到pBuf中
     *
     * @param pCurNode
     * @param pBuf [out]
     *
     * @return
     */
    int saveToBuffer(xmlNode *pCurNode, std::string *pBuf);

    /**
     * @brief 将整个xml文档dump到pBuf
     *
     * @param pBuf [out]
     *
     * @return
     */
    int saveToBuffer(std::string *pBuf);

    /**
     * @brief 将整个xml文档存储到指定文件
     *
     * @param file
     *
     * @return
     */
    int saveToFile(const std::string &file);

    /**
     * @brief 创建一个新的xml文档
     *
     * @param rootName 根节点名
     *
     * @return
     */
    xmlNode *createDocument(const std::string &rootName);

    /**
     * @brief 取得文档根节点
     *
     * @return
     */
    xmlNode *getRootNode() const;

    /**
     * @brief 取父节点
     *
     * @param pCurNode 当前节点
     *
     * @return
     */
    xmlNode *getParentNode(xmlNode *pCurNode) const;

    /**
     * @brief 取得当前节点的第一个子节点
     *        尽量不要使用此函数, 因为xml文件中的换行缩进等均被解析为节点
     *        无法确知哪一个是第一个子节点
     *
     * @param pCurNode
     *
     * @return
     */
    xmlNode *getChildNode(xmlNode *pCurNode) const;

    /**
     * @brief 取当前节点所有子节点
     *
     * @param pCurNode
     * @param pNodeList [out]
     *
     * @return
     */
    int getChildNodes(xmlNode *pCurNode, std::list<xmlNode *> *pNodeList);

    /**
     * @brief 按节点名取得子节点
     *
     * @param pCurNode
     * @param nodeName
     *
     * @return
     */
    xmlNode *getChildNodeByNodeName(xmlNode *pCurNode,
                                    const std::string &nodeName);

    /**
     * @brief 按节点名取得当前节点所有同名子节点
     *
     * @param pCurNode
     * @param nodeName
     * @param pNodeList
     *
     * @return
     */
    int getChildNodesByNodeName(xmlNode *pCurNode,
                                const std::string &nodeName,
                                std::list<xmlNode *> *pNodeList);

    /**
     * @brief 取得当前节点指定子节点的值
     *
     * @param pCurNode
     * @param childNodeName
     * @param pValue [out]
     *
     * @return 
     */
    int getChildNodeValueByNodeName(xmlNode *pCurNode,
                                    const std::string &childNodeName,
                                    std::string *pValue);
    /**
     * @brief 取节点值
     *
     * @param pCurNode
     * @param pValue [out]
     *
     * @return
     */
    int getNodeValue(xmlNode *pCurNode, std::string *pValue);

    /**
     * @brief 设置当前节点值
     *
     * @param pCurNode
     * @param value
     *
     * @return
     */
    int setNodeValue(xmlNode *pCurNode, const std::string &value);

    /**
     * @brief 在pParentNode添加一个名为nodeName,值为nodeValue的节点
     *
     * @param pParentNode
     * @param nodeName
     * @param nodeValue
     *
     * @return
     */
    xmlNode *addChildNode(xmlNode *pParentNode,
                          const std::string &nodeName,
                          const std::string &nodeValue);

    /**
     * @brief 在pParentNode下添加一个名为nodeName的节点
     *
     * @param pParentNode
     * @param nodeName
     *
     * @return
     */
    xmlNode *addChildNode(xmlNode *pParentNode,
                          const std::string &nodeName);

    /**
     * @brief 将pChildNode及其所有子孙节点添加到pParentNode
     *
     * @param pParentNode
     * @param pChildNode
     *
     * @return
     */
    xmlNode *addChildNode(xmlNode *pParentNode, xmlNode *pChildNode);

    /**
     * @brief 使用XPath查询节点
     *
     * @param xpath
     *
     * @return
     */
    xmlNode *getNodeByXPath(const std::string &xpath);

    /**
     * @brief XPath相对路径查询
     *
     * @param pCurNode
     * @param xpath
     *
     * @return
     */
    xmlNode *getNodeByXPath(xmlNode *pCurNode, const std::string &xpath);

    /**
     * @brief 使用XPath查询多个节点
     *
     * @param xpath
     * @param pNodeList [out]
     *
     * @return
     */
    int getNodesByXPath(const std::string &xpath,
                        std::list<xmlNode *> *pNodeList);

    int getNodesByXPath(xmlNode *pCurNode, const std::string &xpath,
                        std::list<xmlNode *> *pNodeList);

    /**
     * @brief 使用XPath查询节点值
     *
     * @param xpath
     * @param pValue [out]
     *
     * @return
     */
    int getNodeValueByXPath(const std::string &xpath, std::string *pValue);

    int getNodeValueByXPath(xmlNode *pCurNode, const std::string &xpath,
                            std::string *pValue);

private:
    xmlDoc *m_pDoc;
};

#endif  // _Xml_H_

