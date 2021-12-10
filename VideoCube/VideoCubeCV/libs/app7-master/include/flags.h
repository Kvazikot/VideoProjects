#ifndef FLAGS_H
#define FLAGS_H


struct t_flags
{
	bool bdisable_timer1;
	bool bdisable_timer2;
	bool bdisable_timer3;
	bool bWire;
    bool bShowGrid;
	bool bColorRendering;
	bool bShading;
	bool bChangedRenderMode;
    bool bScanRendering;
	bool bVarsSurviveMode;
	bool bUsingdll;
	bool bCodeEditChange;
	bool bNeedUpdGlobals;
	bool bNeedUpdObjectList;
    bool bNeedUpdFocus;
    bool bNeedUpdVars;
    bool bFixedStepSim;
	bool bOnlyEntetys;
	bool bDisableUpdateCall;
	bool bReinitObjs;
	bool bMakeLoad;
    bool bIsLoading;
    bool bNeedReinitObjInspector;
    bool bNeedReinitObjInspector2;
    bool bShowHiddenAtrs;
    bool iRenderVersion;

};

struct t_objlistflags
{
	bool bGeom;
	bool bVeh;
	bool bPrim;
    bool bCurves;
	bool bHelp;
	bool bGroups;
	bool bUngroop;
	bool bHide;
	bool bHidden;
};

t_flags& GetGFlags();

#endif
