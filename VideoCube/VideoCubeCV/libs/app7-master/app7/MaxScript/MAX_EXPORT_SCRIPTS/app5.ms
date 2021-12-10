SEL_SETS = #("Ground2","SkySphere" ,"Ground2 SkySphere ", "Biped", "Zamok" )
gScale = 0.05
gCount = 0
gAnimCount = 0
gName = "TTT"
APP5_PATH = "d:\\src\\tmp\\Data\\"
line_script = "d:\src\tmp\Data\oneline.lua"

fn seta o a v=
(
	ss = stringstream("")
	format "seta(\"%\",\"%\",\"%\")" o a v to:ss
	s = ss as string
	WriteStr line_script s
	return s
)

fn make_seta o a v=
(
	ss = stringstream("")
	format "seta(\"%\",\"%\",\"%\")" o a v to:ss
	s = ss as string
	return s
)

fn make_setaffine o pos rot sc=
(
	ss = stringstream("")
	format "set_setaffine(\"%\",\"%\",\"%\"),\"%\"))" o pos rot sc to:ss
	s = ss as string
	return s
)


fn Pnt2Str p=
(
	tmp = p.y
	p.y = p.z
	p.z = tmp
	p.x = -p.x
	ss = stringstream("")
	format "%" p to:ss
	s = ss as string
	return substring s 2 (s.count-2)	
)

-- eulertype_integer 1 = XYZ 2 = XZY 3 = YZX
fn Quat2Str q eulertype_integer=
(
	a = (quatToEuler q order:eulertype_integer)
	ss = stringstream("")
	format "%,%,%" a.x a.y a.z to:ss
	return (ss as string)
)

fn MakeObjString oname classname props=
(
		ss = stringstream("")
		format "UpdateObject(\"%:%{" classname oname to:ss		
		for i=1 to props.count-2 do
		(	 
			var=props[2*i-1]
			val=props[2*i]
			format "%=% " var val to:ss
		)
		format "}\")" to:ss
		s = ss as string
		return s
)

fn keytype_as_int t=
(
	if(t == #auto ) then return 0
	if(t == #smooth ) then return 1
	if(t ==  #linear) then return 2
	if(t == #step ) then return 3
	if(t == #slow ) then return 4
	if(t == #custom ) then return 5	
)


fn SelectPoints points=
(
	names = #()
	for o in objects do 
		if( classof o == Point) then
			append names o.name 
	sort names
	for n in names do 
		append points (getnodebyname n)
	return points
)


fn SelectAnimations animations=
(
		for o in objects do 
		(
			if( classof o == Editable_mesh) then
			(
				bExport = false
				if( (o.rotation.isAnimated == false) and (o.position.isAnimated == false) ) then continue
				
				num_rot = (numKeys o.rotation.controller)
				num_pos = (numKeys o.position.controller)

				if( num_rot>0 ) then bExport = true
				if( num_pos>0) then bExport = true
				
				if( bExport ) then append animations o			
			)
		)
	
)
fn NotUndef v def=
(
	ss = stringstream "a"
	format "%" v to:ss
	vs = (ss as string)	
	if(v == undefined) then return def
	else return vs
)

fn Int2Str i=(return (NotUndef i "0") ) 
fn Float2Str i=(return (NotUndef i "0.0") )

fn write_key f k =
(
	WriteFloat f k.time	
	WriteFloat f k.value
	WriteFloat f k.inTangent
	WriteFloat f k.outTangent 
	WriteLong f (keytype_as_int k.inTangentType) 
	WriteLong f (keytype_as_int k.outTangentType) 
--	print (format "t = % k.value = %		" k.time k.value)				
)

fn write_anim o f=
(
	write_str256 f o.name

	num_rot = (numKeys o.rotation.controller)
	if(num_rot > 0) then
	(
		print (format "num_rot=% 		" num_rot)		
		WriteLong f num_rot
		for k in o.rotation.x_rotation.controller.keys do write_key f k
		for k in o.rotation.y_rotation.controller.keys do write_key f k
		for k in o.rotation.z_rotation.controller.keys do write_key f k		
	)
	else
		WriteLong f 0

	num_pos = (numKeys o.position.controller)
	if(num_pos > 0  and (classof o.position.controller) != tcb_position ) then
	(
		--if( (hasProperty o.position "x_position") ) then
		--num_pos = o.position.x_position.keys.count
		--o = $Pyramid01
		print (format "num_pos keys=%		" num_pos)				
		WriteLong f num_pos
		for k in o.position.x_position.controller.keys do write_key f k
		for k in o.position.y_position.controller.keys do write_key f k		
		for k in o.position.z_position.controller.keys do write_key f k
	)
	else
		WriteLong f 0


	--padding
	for i=0 to 7 do WriteByte f 0xFA
	

)

fn UpdateIni keys DoRepl=
(
		sarr_in=#() -- create array
		ParseIni sarr_in (APP5_PATH + "objects.ini")	
		ModifyKeys sarr_in keys
	    SaveIni sarr_in (APP5_PATH + "objects.ini")	
	
)


fn ExportGeom group_name DoReplacef selset=
(
				gCount = 0
				gName = group_name
				fname=(APP5_PATH + "temp.group")
				fname2=APP5_PATH + gName + ".group"
				if( $SkySphere != undefined) then
				(
					$SkySphere.pos = [951.715,28627.5,2977.34]	
					selectMore $SkySphere 
				)
			    if(selset == undefined) then
				(
					export_selection fname gScale
				)
				else
					export_set selset fname gScale
				if( $SkySphere != undefined) then
				(
					$SkySphere.pos = [204072,28627.5,0]			
					move $SkySphere [-1,0,0]
				)
				deleteFile fname2
				copyFile fname fname2
				ss = stringstream "a"
				format "Group:TTT{ n_obj=% scale=%,%,%  }" gCount gScale gScale gScale to:ss
				keys = #((ss as string))
			    UpdateIni keys DoReplacef
			
  				 ss = stringstream "a"
				 format "Exported: % " gCount to:ss
				 return (ss as string)
)	

fn ExportAnimationF DoReplacef=
(
			animations = #()
			SelectAnimations animations			
			gAnimCount = 0
			gAnimCount = animations.Count			
			print "ExportAnimationF"
			--print animations					
			fname=(APP5_PATH + "animations.anim")
			f=fopen fname "wb"	
			for o in animations do  write_anim o f
			fclose(f)		
			
			ss = stringstream "a"
			format "Animations:TTT{ n=% scale=0.1,0.1,0.1 }" gAnimCount to:ss
			keys = #((ss as string))
			UpdateIni keys DoReplacef
			
			ss = stringstream "a"
			format "Exported: % " gAnimCount to:ss
			return (ss as string)
				
)

fn FindApp5=
(
	hwnd = windows.getDesktopHWND()
	arr=windows.getChildrenHWND hwnd
	for i=1 to arr.count do
	( 
		j=findString arr[i][5] "app5" 
		if( j!=undefined) then	
		if(arr[i][5].count	== 4) then
		(
			return arr[i][1]
		)
	)
)

fn App5Exec w str=
(
	setclipboardText str
    windows.sendMessage w 0x0204 1 1
--	windows.sendMessage w 0x0010 1 1
)

fn write_pnt f x y z =
(
	WriteFloat f x
	WriteFloat f y
	WriteFloat f z
)

fn write_tuple f x y =
(
	WriteFloat f x
	WriteFloat f y

)

fn write_str256 f s=
(
	for i=0 to 256 do
	(
		if( i <= s.count ) then
		(
			ss = s[i]
			if(ss!=undefined) then
			(
				WriteByte f (bit.charAsInt ss)
				print (bit.charAsInt ss)
			)
		)
		else
			WriteByte f 0
	)
)

fn write_str f s=
(
	WriteLong f s.count
	os = ftell f
	for i=0 to s.count do WriteByte f 0
	fseek f os #seek_set 
	WriteString f s
	fseek f (os+s.count) #seek_set	
	--print s
)

fn parse_material o=
(
	ss = stringstream ""
    typ = "standart"
    filen = ""
	if(o.diffuseMap!=undefined) then
	if(hasProperty o "diffuseMap") then
	(
		U_Tiling = o.diffuseMap.coords.U_Tiling
		V_Tiling = o.diffuseMap.coords.V_Tiling
		filen = o.diffuseMap.filename
	)
	ambient =o.ambient
	diffuse = o.diffuse
	specular = o.specular
	--print o.ambient.r o.ambient.g o.ambient.b
	format "type=% name=% file=% U_Tiling=% V_Tiling=% ambient=[%,%,%] diffuse=[%,%,%] specular=[%,%,%] " typ o.name filen U_Tiling V_Tiling ambient.r ambient.g ambient.b diffuse.r diffuse.g diffuse.b specular.r specular.g specular.b to:ss
	

	
	return (ss as string)		
)

fn get_mat_str o=
(
	ss = stringstream ""
	typ = "color"
	U_Tiling = 1.0
	V_Tiling = 1.0
	-- showProperties $.material
	-- showProperties $.material.materialList[1]
	if(o.material!=undefined) then
	if(hasProperty o.material "materialList") then
	(
		typ = "multi"
		format "{type=multi name=% " o.material.name to:ss
		for i = 1 to o.material.materialList.count do --loop through all faces
		(
			id = o.material.materialIDList[i];
			--print id;
			format "{id=% " id to:ss
			s = parse_material o.material.materialList[i];
			format "%} " s to:ss
		)
		format "}" to:ss
    )
	
	if(o.material!=undefined) then
	if(hasProperty o.material "diffuseMap") then
	(
		if(o.material.diffuseMap != undefined)  then
		(
		  U_Tiling = o.material.diffuseMap.coords.U_Tiling
		  V_Tiling = o.material.diffuseMap.coords.V_Tiling
    	  typ = "bitmap"
		  filen = o.material.diffuseMap.filename
		  format "{type=% file=% U_Tiling=% V_Tiling=%} " typ filen U_Tiling V_Tiling to:ss

		)
	)

	if( typ == "color" ) then
      format "{type=% file=% U_Tiling=% V_Tiling=% wire_color=[%,%,%]} " typ "no_file" 0 0 (o.wireColor.r / 255) (o.wireColor.g / 255) (o.wireColor.b / 255) to:ss
		--format "no texture" to:ss
	print (ss as string) 
	return (ss as string)		
)			



fn write_obj o f sf=
(
	sf = 1 

		
	write_str  f o.name
	biped_mode = false
	if( findString o.name "Bip" != undefined) then biped_mode = true
	if( biped_mode ) then
	(
		pos = biped.getTransform o #pos
		pos = pos * sf
		q = biped.getTransform o #rotation
		if(q!= undefined) then
		(
			rot = quatToEuler q 
		)
	)
	else
	(
		
		pos = o.pos * sf
		rot = o.rotation
		rot.x = 0 
		rot.y = 0
		rot.z = 0
	)
		
	write_pnt f rot.x rot.y rot.z
	write_pnt f rot.x rot.y rot.z
	write_pnt f (-pos.x * sf) (pos.z * sf) (pos.y * sf)
	write_pnt f 0.0 0.0 0.0
	WriteLong f (getNumVerts o)
	WriteLong f (getNumFaces o)
	print("num faces")
	print (getNumFaces o)
	print("num vert")
	print (getNumVerts o)
	for v = 1 to getNumVerts o do --loop through all vertices
	(
		vert = getVert o v --get the v-th vertex 
		WriteFloat f (-vert.x)
		WriteFloat f (vert.z)		
		WriteFloat f (vert.y)
		--print vert
	)

	for g = 1 to getNumFaces o do --loop through all faces
	(
		face = getFace o g --get the g-th face
		--print face
		WriteLong f (face.x-1)
		WriteLong f (face.y-1)
		WriteLong f (face.z-1)
		--print face.x
	)
		
    hasMapSupport = (meshop.getMapSupport o 1)
	print hasMapSupport
	for v = 1 to getNumVerts o do --loop through all verts
	(
		if(not biped_mode) then
		(
			--tFace = meshOp.getMapFace o 1 g
			if( hasMapSupport ) then
			(			
				if(v < meshop.getNumMapVerts o 1) then
				(
					uvwVert = meshOp.getMapVert o 1 v
					write_tuple f uvwVert[1] uvwVert[2] 
				)
				else
					write_tuple f 0 0
			)
			else
				write_tuple f 0 0
			--print uvwVert
		)
		else
		(
		  write_tuple f 0 0
		)	
	)
	
	for g = 1 to getNumVerts o do --loop through all faces
	(
		N = getNormal o g
		--print N
		write_pnt f N.x N.y N.z
		--print face.x
	)
	
	--one range
	WriteLong f 1	
	WriteLong f 0
	WriteLong f (o.mesh.numverts-1)
	
	--texture name
	write_str  f (get_mat_str o)
	--write_str  f "no material"
	
	WriteLong f (getNumFaces o)
	for g = 1 to getNumFaces o do --loop through all faces
	(
		WriteLong f (getFaceMatID o g)
		--print (getFaceMatID o g)
	)
	
	--padding
	for i=0 to 7 do WriteByte f 0xFA
	
	
)


fn export_selection fname sf=
(
		print sf
		gCount = 0
		f=fopen fname "wb"	
	    
		for o in selection do 
		(
			print o
			if( classof o == Editable_mesh) then
			(
				print (getNumFaces o)
				print (getNumVerts o)
				write_obj o f sf

				gCount = gCount + 1
			)
			if( classof o == Dummy ) then
			(
				 classname = getUserProp o "classname"
				if( classname == "CVehicle" ) then
				(				
				  print (format "CVehicle:%{name=% pos=%,%,%  trenie=0.050000 delkoof=400.000000 stopSignal=False a=3.700000 b=5.120000 c=0.800000 d=0.295000 max_wang=45.000000} "  o.name o.name -o.pos.x o.pos.z o.pos.y )		 
				  ss = stringstream "a"
				  format "CVehicle:%{name=% pos=%,%,% kurs=%,%,% trenie=0.050000 delkoof=400.000000 stopSignal=False a=3.700000 b=5.120000 c=0.800000 d=0.295000 max_wang=45.000000} "  o.name o.name -o.pos.x o.pos.z o.pos.y (quatToEuler o.rotation).z  0 0  to:ss
				  format "\n" to:ss
				  format "CarAutopilot:autopilot%{veh_name=% curve_name=Line404 T1=0.500000 T2=5.000000 Ta=2.0 }"  o.name o.name to:ss
				  format "\n" to:ss

					keys = #((ss as string))
				    UpdateIni keys true				
				)
			)
			if( classof o == line ) then
			(
				keys = #(exportCurve o)
				UpdateIni keys true
			)

		)
		sz = ftell(f) / (1024 * 1024)
		print (format "FileSize % MB   "  sz)		
		fclose(f)
	
)

fn export_all fname sf=
(
		f=fopen fname "wb"
		for o in objects do 
		(
			print o
			if( classof o == Editable_mesh) then
			(
				write_obj o f sf
				gCount = gCount + 1
			)
		)
		sz = ftell(f) / (1024 * 1024)
		print (format "FileSize % MB   "  sz)		
		fclose(f)
	
)

fn WriteStr fname s=
(
	f=fopen fname "w+"
	WriteString f s	
	fseek f -1 #seek_cur 	
	WriteByte f  0x0D
	WriteByte f  0x0A	
	fclose(f)
)


fn WriteStrList fname s=
(
	f=fopen fname "w+"
	for i=1 to s.count do
	(
		WriteString f s[i]	
		fseek f -1 #seek_cur 	
		WriteByte f  0x0D
		WriteByte f  0x0A			
	)		
	WriteByte f  0x0D
	WriteByte f  0x0A	
	fclose(f)
)

--copyFile fname fname2


ExportGeom gName true undefined
