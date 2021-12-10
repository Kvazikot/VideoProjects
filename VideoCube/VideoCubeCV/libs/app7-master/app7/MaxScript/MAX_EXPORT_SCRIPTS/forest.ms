----------------------------------
MaxH = 1000.0
MinH = 500.0
Nid = 3

----------------------------
ground = #()
pyr = #()
verts = #()
aDeletedObjects = #()

for o in objects do 
(
	if( o.name == "Plane01") then
			ground = o
	if( o.name == "Pyramid01")	then
	  pyr = o
	if(( (findString o.name "Pyramid") != undefined )  and (o.name !="Pyramid01")) then		
			append aDeletedObjects o	
)
delete aDeletedObjects

--getInterface polyOp
--return

for f in ground.faces do
(
	--getPropNames f
	--print f.index
	tFace = polyOp.getMapFace ground 1 f.index
	N = polyOp.getFaceMatID ground f.index
	if (N == Nid) then
	(
		verts = (polyOp.getFaceVerts ground f.index) 
		--print (classof verts)
		verts2 = #() as Array
		verts2[1] = ( polyOp.getVert ground verts[1] )
		verts2[2] = ( polyOp.getVert ground verts[2] )
		verts2[3] = ( polyOp.getVert ground verts[3] )
		--verts2[4] = ( polyOp.getVert ground verts[4] )
		
		Cx = 0;  Cy=0; Cz=0;
		for j=1 to 3 do 
		(
			Cx = Cx + verts2[j].x;
			Cy = Cy + verts2[j].y;
			Cz = Cz  + verts2[j].z;			
		)
		Cx = Cx / 3;
		Cy = Cy / 3;
		Cz = Cz / 3;
		
		--polyOp.getVet
		pyr2 = copy pyr;
		pyr2.pos = [Cx,Cy,Cz]
		--pyr2.Height = random MinH MaxH
		s = random MinH MaxH
		pyr2.scale = [1,1,s]

	

	)

	--print o
/* 	if( classof o == Point) then
* 	(
* 		print o.pos
* 		ss = stringstream "a"
* 		format "VPnt:%{ p=% isCanRender = true }" o.name (Pnt2Str(o.pos)) to:ss
* 		append new_strs (ss as string)						
* 	)
*/
)
