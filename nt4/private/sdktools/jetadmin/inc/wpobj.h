/* This header file machine-generated by mktyplib.exe */
/* Interface to type library: WPObj */

#ifndef _WPObj_H_
#define _WPObj_H_

DEFINE_GUID(LIBID_WPObj,0x536ABCA0,0x9240,0x11CF,0x9E,0xD3,0x00,0xAA,0x00,0x4C,0x12,0x0C);

DEFINE_GUID(IID_IWPObj,0xEDD8BBC0,0x9240,0x11CF,0x9E,0xD3,0x00,0xAA,0x00,0x4C,0x12,0x0C);

/* Definition of interface: IWPObj */
#undef INTERFACE
#define INTERFACE IWPObj

DECLARE_INTERFACE_(IWPObj, IDispatch)
{
#ifndef NO_BASEINTERFACE_FUNCS

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;

    STDMETHOD(GetTypeInfo)(
      THIS_
      UINT itinfo,
      LCID lcid,
      ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetIDsOfNames)(
      THIS_
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID FAR* rgdispid) PURE;

    STDMETHOD(Invoke)(
      THIS_
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      UINT FAR* puArgErr) PURE;
#endif

    /* IWPObj methods */
    STDMETHOD(Application)(THIS_ IWPObj FAR* FAR* retval) PURE;
    STDMETHOD(WpPostFile)(THIS_ long hwnd, BSTR lppszLocalPaths, long FAR* lpcbSiteName, BSTR FAR* lpszSiteName, long FAR* lpcbURL, BSTR FAR* lpszURL, long dwFlag, long FAR* lpRetCode) PURE;
};

DEFINE_GUID(CLSID_WPObj,0x53DEFDE0,0x9222,0x11CF,0x9E,0xD3,0x00,0xAA,0x00,0x4C,0x12,0x0C);

#ifdef __cplusplus
class WPObj;
#endif

#endif