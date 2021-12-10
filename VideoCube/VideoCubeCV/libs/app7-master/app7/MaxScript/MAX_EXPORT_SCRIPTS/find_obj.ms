fn findObj name =
(
	for o in objects do 
		  if( findString o.name name != undefined) then
		     return o
)
			  