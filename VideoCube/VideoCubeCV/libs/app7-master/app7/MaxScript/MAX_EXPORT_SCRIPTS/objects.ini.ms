sarr_in=#() -- create array
--ParseIni sarr_in "E:\\src\\app5\\Debug\\object
--key = "Group:TTT{ n_obj=444 scale=0.01,0.01,0.01 }"s.ini" 
--SetProps "sasas" props
--SetAttrs "cam01" atrs
--ModifyKey sarr_in key
--SaveIni  "E:\\src\\app5\\Debug\\objects2.ini" 
--shellLaunch "notepad.exe" "E:\\src\\app5\\Debug\\objects2.ini" 

fn SetUserProps=
(
		for o in objects do 
		(
			s = GetObjDescr(o.name)
			if(s!="") then
			(
				vars = (GetVars s)
				for var in vars do 
				(
					val = (GetVal var s vars)
					setUserProp o var val					
				)
				format "(Updating props for %)" o.name
				
			)
			
		)			
)	

fn SetProps s vars props=
(
		--t = (GetVars s)
		for i=1 to props.count-2 do
		(	 
			--print i
			var=props[2*i-1]
			val=props[2*i]
			
			--print var
			if ( (var!=undefined) and (val!=undefined) and (s!=undefined)  ) then
			(
				s = (SetVal s vars var val)
				--format "(WEWE %)" s
			)
			--print s
		)
		return s
)
			

fn SetAttrs oname props=
(
	s = GetObjDescr(oname)
	i=GetDescrIdx(oname)	
	vars = (GetVars s)
	s=(SetProps s vars props)	
	
	--update descr
	--if(i==-1) then return ""
	--if(i>sarr_in.count) then return ""
	--format "(WEWE %)" s
	sarr_in[i] = s
	
	return s
)

		

fn SetVal s table var val=
(
	idx=(GetVarIdx var table)	
		
	if( idx==-1 ) then return s
	if( (idx+1) > table.count ) then return s
	nex_var=table[idx+1]
	
	k= findString s var
	j = findString s nex_var
	
	pair=substring s k (j-k-1)
	toks = filterString pair "="
	new_pair=toks[1] + "=" + val
	s_new=replace s k (j-k-1) new_pair
	return s_new
)


fn GetDescrIdx oname=
(
	for i=1 to sarr_in.count do
	( 
		s = sarr_in[i]
		if( oname == ObjName(s)	) then return i
	)
	return -1		
)


fn GetObjDescr oname=
(
	for i=1 to sarr_in.count do
	( 
		s = sarr_in[i]
		if( oname == ObjName(s)	) then return s
	)
	return ""		
)



fn GetVars s=
(
	vars=#()
	j = findString s "{"
	k = findString s "}"
	if (j!=undefined) and (k!=undefined)  then
	(		
		s2=substring s (j+1) (k-j-1)
		toks = filterString s2 "="
		for m=1 to toks.count do
		(
			toks2 = filterString toks[m] " "
			if(toks2.count > 0) then
			(
				if(toks2[toks2.count]!=" ") and (toks2[toks2.count]!="") then
					append vars toks2[toks2.count]				
			)
		)
	)

	return vars

)

fn GetVarIdx var table=
(
	for i=1 to table.count do
	( 
		if ( table[i] == var) then
		  return i
	)
	return -1
	
)

fn GetVal var s table=
(
	idx=(GetVarIdx var table)	
	if( idx==-1 ) then return ""
	if( (idx+1) > table.count ) then return ""
	nex_var=table[idx+1]
	k= findString s var
	j = findString s nex_var
	pair=substring s k (j-k-1)
	toks = filterString pair "="
	return toks[2]
)



fn ObjName s=
(
	j = findString s "{"
	if(j!=undefined) then
	(
		s2=substring s 1 (j-1)
		tokens = filterString s2 ":"
		if(tokens.count > 1) then return tokens[2] else return " "				
	)
)

fn ObjClass s=
(
	j = findString s "{"
	if(j!=undefined) then
	(
		s2=substring s 1 (j-1)
		tokens = filterString s2 ":"
		if(tokens.count > 0) then return tokens[1] else return " "					
	)	

)


fn ModifyKey sarr_in key=
(
	searchName = ObjName(key)
	searchClass = ObjClass(key)
	for i=1 to sarr_in.count do
	( 
		s = sarr_in[i]
		if( searchName == ObjName(s)	) then 
		if( searchClass == ObjClass(s)	) then 		
		(
			sarr_in[i] = key
			return ""
		)
	)
	append sarr_in key
	return ""		
)


fn ModifyKeys sarr_in keys=
(
	for i=1 to keys.count do
		ModifyKey sarr_in keys[i]
	return ""		
)

fn CheckFSize fname=
(
	f=fopen fname "rt"
	fseek f 0 #seek_end 	
	fsize=ftell f
	fclose f
	if(f == undefined) then return false
	if(fsize==undefined) then return false
	if(fsize<10) then return false	
	return true
	
)

fn ParseIni sarr_in fname=
(
	if( CheckFSize(fname) == false ) then 
	(
		sarr_in=#("--max script generated"," ") -- create array		
		SaveIni sarr_in fname
		return 1
	)
	--f=openFile fname
	f=fopen fname "rt"
	fseek f 0 #seek_end 	
	fsize=ftell f
	fseek f 0 #seek_set 	
	fsize = fsize - 2
	print fsize
	pos=0
	i=1
	s=" "
	while (i<fsize) do
	(
		b = 0
		b = ReadByte f			
		--print b
		if( b != undefined) then
		(		
			if( b == 0x0A ) then
			(
				if(s.count > 10) then
					append sarr_in s
				s = ""
			)
			else
			(
				if(b > 31) then
				(
					c = bit.intAsChar b
					s = s + c
					
				)
			)
		)
				
		i = i + 1
	)
	fclose(f)
)

fn SaveIni sarr_in fname=
(
	f=fopen fname "w+"
	for i=1 to sarr_in.count do
	(
		if(sarr_in[i].count < 10	) then continue;
		
		WriteString f (sarr_in[i] )
		fseek f -1 #seek_cur 	
		WriteByte f  0x0D
		WriteByte f  0x0A			
		WriteByte f  0x0D
		WriteByte f  0x0A			
		
	)		
	WriteByte f  0x0D
	WriteByte f  0x0A	
	fclose(f)
)


