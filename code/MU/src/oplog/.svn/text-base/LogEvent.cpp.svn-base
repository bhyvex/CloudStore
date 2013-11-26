/*
 * @file LogEvent.cpp
 * @brief Log record.
 *
 * @version 1.0
 * @date Mon Jul  9 15:27:07 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "LogEvent.h"

#include "protocol/protocol.h"
#include "data/FileMeta.h"

#include <inttypes.h>

#include <string>
#include <list>

LogEvent::~LogEvent()
{

}

LogEvent::LogEvent()
{
    m_SeqNr = 0;
    m_UserId = 0;
    m_BucketId = 0;
    m_OpCode = 0;
}

bool
LogEvent::serializeToString(std::string *pData)
{
    cstore::Log_Entry entry;

    entry.set_seq_nr(m_SeqNr);
    entry.set_uid(m_UserId);
    entry.set_token(m_Token);
    entry.set_op_code(m_OpCode);

    if (MU_OP_MOV_DIR == m_OpCode || MU_OP_MOV_FILE == m_OpCode) {
        entry.set_src_path(m_SrcPath);
        entry.set_dest_path(m_DestPath);

    } else {
        entry.set_path(m_Path);
    }

    if (MU_OP_PUT_FILE == m_OpCode) {
        cstore::File_Meta *pFileMeta;
        cstore::File_Attr *pFileAttr;
        cstore::Block_Meta *pBlockMeta;

        pFileMeta = entry.mutable_file_meta();
        pFileAttr = pFileMeta->mutable_attr();
        pFileAttr->set_version(m_FileMeta.m_Attr.m_Version);
        pFileAttr->set_mode(m_FileMeta.m_Attr.m_Mode);
        pFileAttr->set_ctime(m_FileMeta.m_Attr.m_CTime);
        pFileAttr->set_mtime(m_FileMeta.m_Attr.m_MTime);
        pFileAttr->set_type(m_FileMeta.m_Attr.m_Type);
        pFileAttr->set_size(m_FileMeta.m_Attr.m_Size);

        for (std::list<BlockMeta>::iterator it = m_FileMeta.m_BlockList.begin();
             it != m_FileMeta.m_BlockList.end(); ++it) {
            pBlockMeta = pFileMeta->add_block_list();
            pBlockMeta->set_checksum(it->m_Checksum);
        }
    }

    return entry.SerializeToString(pData);
}


