#include "stdafx.h"
#include "CXMLBuilder.h"

#define _OSMETHOD_CALL "methodCall"
#define _OSMETHOD_NAME "methodName"
#define _OSPARAM       "param"
#define _OSPARAMS      "params"
#define _OSVALUE       "value"
#define _OSSTRING      "string"
#define _OSMETHOD_RESP "methodResponse"
#define _OSSTRUCT      "struct"
#define _OSMEMBER      "member"
#define _OSNAME        "name"
#define _OSTOKEN       "token"
#define _OSARRAY       "array"
#define _OSDATA        "data"

CXMLBuilder::CXMLBuilder(const std::wstring &user, const std::wstring &pass, const std::wstring &user_agent)
	:m_user(user), m_pass(pass), m_user_agent(user_agent)
{

}

bool CXMLBuilder::BuildLogin(XMLMemoryWriter &xml_memory_writer)
{
	if (m_user_agent[0] != '\0')
	{
		pugi::xml_document xml_doc;
		xml_doc.append_child(_OSMETHOD_CALL).append_child(_OSMETHOD_NAME).append_child(pugi::node_pcdata).set_value("LogIn");
		pugi::xml_node params_node = xml_doc.child(_OSMETHOD_CALL).append_child(_OSPARAMS);
		params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(m_user).c_str());
		params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(m_pass).c_str());
		params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value("eng");
		params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(m_user_agent).c_str());
		xml_doc.save(xml_memory_writer, "\t", pugi::format_default, pugi::encoding_utf8);
		return true;
	}
	else
		return false;
}

bool CXMLBuilder::BuildLogout(XMLMemoryWriter &xml_memory_writer)
{
	if (m_token[0] != '\0')
	{
		pugi::xml_document xml_doc;
		xml_doc.append_child(_OSMETHOD_CALL).append_child(_OSMETHOD_NAME).append_child(pugi::node_pcdata).set_value("LogOut");
		pugi::xml_node params_node = xml_doc.child(_OSMETHOD_CALL).append_child(_OSPARAMS);
		params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(m_token.c_str());
		xml_doc.save(xml_memory_writer, "\t", pugi::format_default, pugi::encoding_utf8);
		return true;
	}
	else
		return false;
}

bool CXMLBuilder::BuildSearch(XMLMemoryWriter& xml_memory_writer,InputFileInfo &file_info,const std::wstring lang)
{
	pugi::xml_document xml_doc;
	pugi::xml_node     member_node;
	pugi::xml_node     struct_node;
	pugi::xml_node     array_data;
	xml_doc.append_child(_OSMETHOD_CALL).append_child(_OSMETHOD_NAME).append_child(pugi::node_pcdata).set_value("SearchSubtitles");
	pugi::xml_node params_node = xml_doc.child(_OSMETHOD_CALL).append_child(_OSPARAMS);
	//---
	params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(m_token.c_str());
	//---
	array_data = params_node.append_child(_OSPARAM).append_child(_OSVALUE).append_child(_OSARRAY).append_child("data");
	//---
	struct_node = array_data.append_child(_OSVALUE).append_child(_OSSTRUCT);

	member_node = struct_node.append_child(_OSMEMBER);
	member_node.append_child(_OSNAME).append_child(pugi::node_pcdata).set_value("sublanguageid");
	member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(lang).c_str());

	member_node = struct_node.append_child(_OSMEMBER);
	member_node.append_child(_OSNAME).append_child(pugi::node_pcdata).set_value("moviehash");
	member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(file_info.file_hash).c_str());

	member_node = struct_node.append_child(_OSMEMBER);
	member_node.append_child(_OSNAME).append_child(pugi::node_pcdata).set_value("moviebytesize");
	member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(file_info.file_size).c_str());

	struct_node = array_data.append_child(_OSVALUE).append_child(_OSSTRUCT);

	member_node = struct_node.append_child(_OSMEMBER);
	member_node.append_child(_OSNAME).append_child(pugi::node_pcdata).set_value("sublanguageid");
	member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(lang).c_str());

	member_node = struct_node.append_child(_OSMEMBER);
	member_node.append_child(_OSNAME).append_child(pugi::node_pcdata).set_value("query");

	member_node.append_child(_OSVALUE).append_child(_OSSTRING).append_child(pugi::node_pcdata).set_value(Tools::WStringToString(file_info.file_name_no_extension).c_str());
	//--- 
	xml_doc.save(xml_memory_writer, "\t", pugi::format_default, pugi::encoding_utf8);

	return true;
}

bool CXMLBuilder::IsResponseOK(Buffer& response)
{
	pugi::xml_document xml_doc;
	pugi::xml_parse_result result = xml_doc.load_buffer(response.buffer_in, response.buffer_in_total);
	if (result)
	{
		pugi::xml_node struct_node = xml_doc.child(_OSMETHOD_RESP).child(_OSPARAMS).child(_OSPARAM).child(_OSVALUE).child(_OSSTRUCT);
		for (pugi::xml_node member_node = struct_node.child(_OSMEMBER); member_node; member_node = member_node.next_sibling(_OSMEMBER))
		{
			if (strcmp(member_node.child(_OSNAME).child_value(), "status") == 0)
			{
				if (strcmp(member_node.child(_OSVALUE).child(_OSSTRING).child_value(), "200 OK") == 0)
					return true;
			}
		}
	}
	return false;
}

bool CXMLBuilder::GetToken(Buffer& response)
{
	pugi::xml_document xml_doc;
	pugi::xml_parse_result result = xml_doc.load_buffer(response.buffer_in, response.buffer_in_total);
	if (result)
	{
		pugi::xml_node struct_node = xml_doc.child(_OSMETHOD_RESP).child(_OSPARAMS).child(_OSPARAM).child(_OSVALUE).child(_OSSTRUCT);
		for (pugi::xml_node member_node = struct_node.child(_OSMEMBER); member_node; member_node = member_node.next_sibling(_OSMEMBER))
		{
			if (strcmp(member_node.child(_OSNAME).child_value(), _OSTOKEN) == 0)
			{
				char token[64];
				strcpy_s(token, 64, member_node.child(_OSVALUE).child(_OSSTRING).child_value());
				m_token.assign(token);
				break;
			}
		}
	}
	return m_token[0]!='\0';
}

bool CXMLBuilder::GetSubtitlesFound(IOpenSubtitlesAPI::SubtitleInfoList &sub_list, Buffer& response, IEventListener* event_listener)
{
	pugi::xml_document xml_doc;
	pugi::xml_parse_result result = xml_doc.load_buffer(response.buffer_in, response.buffer_in_total, pugi::parse_default);
	if (!result)
		return(false);
	//--- save struct node
	pugi::xml_node struct_node = xml_doc.child(_OSMETHOD_RESP).child(_OSPARAMS).child(_OSPARAM).child(_OSVALUE).child(_OSSTRUCT);
	//--- find data member
	pugi::xml_node member_node;
	for (member_node = struct_node.child(_OSMEMBER); member_node; member_node = member_node.next_sibling(_OSMEMBER))
	{
		if (strcmp(member_node.child(_OSNAME).child_value(), _OSDATA) == 0)
			break;
	}
	//--- find data node
	pugi::xml_node data_node = member_node.child(_OSVALUE).child(_OSARRAY).child(_OSDATA);
	//--- go through all values
    int count = 0;
	for (pugi::xml_node value_node = data_node.child(_OSVALUE); value_node; value_node = value_node.next_sibling(_OSVALUE))
	{
		IOpenSubtitlesAPI::subtitle_info current_info;
		pugi::xml_node struct_node = value_node.child(_OSSTRUCT);
		for (member_node = struct_node.child(_OSMEMBER); member_node; member_node = member_node.next_sibling(_OSMEMBER))
		{
			std::string name = member_node.child(_OSNAME).child_value();
			std::wstring value = Tools::StringToWString(member_node.child(_OSVALUE).child(_OSSTRING).child_value());
            if (name == "MatchedBy")
                current_info.matched_by = value;
            else
                if (name == "SubFormat")
                    current_info.format = value;
                else
                    if (name == "SubDownloadsCnt")
                        current_info.download_count = value;
                    else
                        if (name == "MovieReleaseName")
                            current_info.release_name = value;
                        else
                            if (name == "ZipDownloadLink")
                                current_info.zip_link = value;
                            else
                                if (name == "SubFileName")
                                    current_info.sub_file_name = value;
		}
		sub_list.push_back(current_info);
        event_listener->OnSubtitle(current_info);
        if (++count == 6)
            break;
	}
	return true;
}
