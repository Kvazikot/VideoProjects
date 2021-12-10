fn findObj name =
(
	for o in objects do 
		  if( findString o.name name != undefined) then
		     return o
)

obj = findObj "Box001"
mypath = findObj "Line001"
knotCount = numknots mypath
for i=1 to knotCount do
(	 
	p = (getKnotPoint mypath 1 i)	    
    in_vec = (getInVec mypath 1 i)
    out_vec = (getOutVec mypath 1 i)
	print in_vec
	--print out_vec
	--maxOps.cloneNodes obj cloneType:#instance newNodes:&nnl
	--nnl.pos = [p.x,p.y,p.z]
    --print (getKnotType $ 1 i)	
)

