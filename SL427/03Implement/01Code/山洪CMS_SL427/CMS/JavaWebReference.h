//
// sproxy.exe generated file
// do not modify this file
//
// Created: 07/10/2023@23:10:22
//

#pragma once


#if !defined(_WIN32_WINDOWS) && !defined(_WIN32_WINNT) && !defined(_WIN32_WCE)
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace StubWebService
{

template <typename TClient = CSoapSocketClientT<> >
class CStubWebServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CStubWebServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://192.168.0.34:8080/gmsastub/services/StubWebService.StubWebServiceHttpSoap11Endpoint/"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CStubWebServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT execDML(
		BSTR username, 
		BSTR password, 
		BSTR sqls, 
		BSTR* __retval
	);

	HRESULT query(
		BSTR username, 
		BSTR password, 
		BSTR sqls, 
		BSTR* __retval
	);

	HRESULT execDDL(
		BSTR username, 
		BSTR password, 
		BSTR sqls, 
		BSTR* __retval
	);

	HRESULT sayHello(
		BSTR text, 
		BSTR* __retval
	);
};

typedef CStubWebServiceT<> CStubWebService;

struct __CStubWebService_execDML_struct
{
	BSTR username;
	BSTR password;
	BSTR sqls;
	BSTR __retval;
};

extern __declspec(selectany) const _soapmapentry __CStubWebService_execDML_entries[] =
{

	{
		0x82658340, 
		"username", 
		L"username", 
		sizeof("username")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDML_struct, username),
		NULL,
		NULL,
		-1,
	},
	{
		0x9C439233, 
		"password", 
		L"password", 
		sizeof("password")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDML_struct, password),
		NULL,
		NULL,
		-1,
	},
	{
		0x0040FEA3, 
		"sqls", 
		L"sqls", 
		sizeof("sqls")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDML_struct, sqls),
		NULL,
		NULL,
		-1,
	},
	{
		0x11515F60, 
		"return", 
		L"return", 
		sizeof("return")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDML_struct, __retval),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CStubWebService_execDML_map =
{
	0xC8DF67B1,
	"execDML",
	L"execDMLResponse",
	sizeof("execDML")-1,
	sizeof("execDMLResponse")-1,
	SOAPMAP_FUNC,
	__CStubWebService_execDML_entries,
	sizeof(__CStubWebService_execDML_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0x0E4FD737,
	"http://axis.service.securityrail.jdgm.com",
	L"http://axis.service.securityrail.jdgm.com",
	sizeof("http://axis.service.securityrail.jdgm.com")-1
};


struct __CStubWebService_query_struct
{
	BSTR username;
	BSTR password;
	BSTR sqls;
	BSTR __retval;
};

extern __declspec(selectany) const _soapmapentry __CStubWebService_query_entries[] =
{

	{
		0x82658340, 
		"username", 
		L"username", 
		sizeof("username")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_query_struct, username),
		NULL,
		NULL,
		-1,
	},
	{
		0x9C439233, 
		"password", 
		L"password", 
		sizeof("password")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_query_struct, password),
		NULL,
		NULL,
		-1,
	},
	{
		0x0040FEA3, 
		"sqls", 
		L"sqls", 
		sizeof("sqls")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_query_struct, sqls),
		NULL,
		NULL,
		-1,
	},
	{
		0x11515F60, 
		"return", 
		L"return", 
		sizeof("return")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_query_struct, __retval),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CStubWebService_query_map =
{
	0x01814E45,
	"query",
	L"queryResponse",
	sizeof("query")-1,
	sizeof("queryResponse")-1,
	SOAPMAP_FUNC,
	__CStubWebService_query_entries,
	sizeof(__CStubWebService_query_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0x0E4FD737,
	"http://axis.service.securityrail.jdgm.com",
	L"http://axis.service.securityrail.jdgm.com",
	sizeof("http://axis.service.securityrail.jdgm.com")-1
};


struct __CStubWebService_execDDL_struct
{
	BSTR username;
	BSTR password;
	BSTR sqls;
	BSTR __retval;
};

extern __declspec(selectany) const _soapmapentry __CStubWebService_execDDL_entries[] =
{

	{
		0x82658340, 
		"username", 
		L"username", 
		sizeof("username")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDDL_struct, username),
		NULL,
		NULL,
		-1,
	},
	{
		0x9C439233, 
		"password", 
		L"password", 
		sizeof("password")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDDL_struct, password),
		NULL,
		NULL,
		-1,
	},
	{
		0x0040FEA3, 
		"sqls", 
		L"sqls", 
		sizeof("sqls")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDDL_struct, sqls),
		NULL,
		NULL,
		-1,
	},
	{
		0x11515F60, 
		"return", 
		L"return", 
		sizeof("return")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_execDDL_struct, __retval),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CStubWebService_execDDL_map =
{
	0xA4804D88,
	"execDDL",
	L"execDDLResponse",
	sizeof("execDDL")-1,
	sizeof("execDDLResponse")-1,
	SOAPMAP_FUNC,
	__CStubWebService_execDDL_entries,
	sizeof(__CStubWebService_execDDL_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0x0E4FD737,
	"http://axis.service.securityrail.jdgm.com",
	L"http://axis.service.securityrail.jdgm.com",
	sizeof("http://axis.service.securityrail.jdgm.com")-1
};


struct __CStubWebService_sayHello_struct
{
	BSTR text;
	BSTR __retval;
};

extern __declspec(selectany) const _soapmapentry __CStubWebService_sayHello_entries[] =
{

	{
		0x00415985, 
		"text", 
		L"text", 
		sizeof("text")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_sayHello_struct, text),
		NULL,
		NULL,
		-1,
	},
	{
		0x11515F60, 
		"return", 
		L"return", 
		sizeof("return")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CStubWebService_sayHello_struct, __retval),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CStubWebService_sayHello_map =
{
	0x97CBEDD0,
	"sayHello",
	L"sayHelloResponse",
	sizeof("sayHello")-1,
	sizeof("sayHelloResponse")-1,
	SOAPMAP_FUNC,
	__CStubWebService_sayHello_entries,
	sizeof(__CStubWebService_sayHello_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0x0E4FD737,
	"http://axis.service.securityrail.jdgm.com",
	L"http://axis.service.securityrail.jdgm.com",
	sizeof("http://axis.service.securityrail.jdgm.com")-1
};

extern __declspec(selectany) const _soapmap * __CStubWebService_funcs[] =
{
	&__CStubWebService_execDML_map,
	&__CStubWebService_query_map,
	&__CStubWebService_execDDL_map,
	&__CStubWebService_sayHello_map,
	NULL
};

template <typename TClient>
inline HRESULT CStubWebServiceT<TClient>::execDML(
		BSTR username, 
		BSTR password, 
		BSTR sqls, 
		BSTR* __retval
	)
{
    if ( __retval == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CStubWebService_execDML_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.username = username;
	__params.password = password;
	__params.sqls = sqls;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:execDML\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*__retval = __params.__retval;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CStubWebServiceT<TClient>::query(
		BSTR username, 
		BSTR password, 
		BSTR sqls, 
		BSTR* __retval
	)
{
    if ( __retval == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CStubWebService_query_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.username = username;
	__params.password = password;
	__params.sqls = sqls;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:query\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*__retval = __params.__retval;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CStubWebServiceT<TClient>::execDDL(
		BSTR username, 
		BSTR password, 
		BSTR sqls, 
		BSTR* __retval
	)
{
    if ( __retval == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CStubWebService_execDDL_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.username = username;
	__params.password = password;
	__params.sqls = sqls;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:execDDL\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*__retval = __params.__retval;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CStubWebServiceT<TClient>::sayHello(
		BSTR text, 
		BSTR* __retval
	)
{
    if ( __retval == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CStubWebService_sayHello_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.text = text;

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"urn:sayHello\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*__retval = __params.__retval;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CStubWebServiceT<TClient>::GetFunctionMap()
{
	return __CStubWebService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CStubWebServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CStubWebService_execDML_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CStubWebService_execDML_atlsoapheader_map = 
	{
		0xC8DF67B1,
		"execDML",
		L"execDMLResponse",
		sizeof("execDML")-1,
		sizeof("execDMLResponse")-1,
		SOAPMAP_HEADER,
		__CStubWebService_execDML_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0x0E4FD737,
		"http://axis.service.securityrail.jdgm.com",
		L"http://axis.service.securityrail.jdgm.com",
		sizeof("http://axis.service.securityrail.jdgm.com")-1
	};

	static const _soapmapentry __CStubWebService_query_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CStubWebService_query_atlsoapheader_map = 
	{
		0x01814E45,
		"query",
		L"queryResponse",
		sizeof("query")-1,
		sizeof("queryResponse")-1,
		SOAPMAP_HEADER,
		__CStubWebService_query_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0x0E4FD737,
		"http://axis.service.securityrail.jdgm.com",
		L"http://axis.service.securityrail.jdgm.com",
		sizeof("http://axis.service.securityrail.jdgm.com")-1
	};

	static const _soapmapentry __CStubWebService_execDDL_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CStubWebService_execDDL_atlsoapheader_map = 
	{
		0xA4804D88,
		"execDDL",
		L"execDDLResponse",
		sizeof("execDDL")-1,
		sizeof("execDDLResponse")-1,
		SOAPMAP_HEADER,
		__CStubWebService_execDDL_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0x0E4FD737,
		"http://axis.service.securityrail.jdgm.com",
		L"http://axis.service.securityrail.jdgm.com",
		sizeof("http://axis.service.securityrail.jdgm.com")-1
	};

	static const _soapmapentry __CStubWebService_sayHello_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CStubWebService_sayHello_atlsoapheader_map = 
	{
		0x97CBEDD0,
		"sayHello",
		L"sayHelloResponse",
		sizeof("sayHello")-1,
		sizeof("sayHelloResponse")-1,
		SOAPMAP_HEADER,
		__CStubWebService_sayHello_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0x0E4FD737,
		"http://axis.service.securityrail.jdgm.com",
		L"http://axis.service.securityrail.jdgm.com",
		sizeof("http://axis.service.securityrail.jdgm.com")-1
	};


	static const _soapmap * __CStubWebService_headers[] =
	{
		&__CStubWebService_execDML_atlsoapheader_map,
		&__CStubWebService_query_atlsoapheader_map,
		&__CStubWebService_execDDL_atlsoapheader_map,
		&__CStubWebService_sayHello_atlsoapheader_map,
		NULL
	};
	
	return __CStubWebService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CStubWebServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CStubWebServiceT<TClient>::GetNamespaceUri()
{
	return L"http://axis.service.securityrail.jdgm.com";
}

template <typename TClient>
ATL_NOINLINE inline const char * CStubWebServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CStubWebServiceT<TClient>::GetNamespaceUriA()
{
	return "http://axis.service.securityrail.jdgm.com";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CStubWebServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CStubWebServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace StubWebService
