fn exportCurve o=
(
	knotCount = numknots o
	ss = stringstream("")

	format "%_DATA{ " o.name to:ss	
	
	format "pts=[" to:ss	
	for i=1 to knotCount do
	(	 
		p = (getKnotPoint $ 1 i)
	    format "%,%,% " -p.x p.z p.y to:ss		
	    --print (getKnotType $ 1 i)	
	)
    format "] " to:ss	
	
	format "vin=[" to:ss	
	for i=1 to knotCount do
	(	 
	   v = (getInVec $ 1 i)
	   format "%,%,% " -v.x v.z v.y to:ss			   
	)
	format "] " to:ss	
	format "vout=[" to:ss	
	for i=1 to knotCount do
	(	 
	   v = (getOutVec $ 1 i)
	   format "%,%,% " -v.x v.z v.y to:ss			   
	)
	format "] }" to:ss	
    format "\n" to:ss	
	format "MaxCurve:%{ }" o.name to:ss	

	
	return ss as string
)


