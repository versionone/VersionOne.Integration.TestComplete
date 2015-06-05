// dllmain.h : Declaration of module class.

class CVersionOneTestCompletePluginModule : public CAtlDllModuleT< CVersionOneTestCompletePluginModule >
{
public :
	DECLARE_LIBID(LIBID_VersionOneTestCompletePluginLib)
};

extern class CVersionOneTestCompletePluginModule _AtlModule;
extern HINSTANCE hInstance;