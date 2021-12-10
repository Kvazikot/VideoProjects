--bip = $'Bip001 L Foot'.controller
--k = biped.getKey $'Bip001 L Foot'.controller 3 
--showProperties k 

--p = biped.getTransform $'Bip001 L Foot' #pos
--p.x
o = $'Bip001'
biped.getTransform o #pos
N_frames = 24
for frame_n=0 to N_frames do  
(
	q = biped.getTransform o #rotation
	eu = quatToEuler q order:1 
	biped.setTransform o #pos [40,0,112.8] true
	biped.setTransform o #rotation (eulerAngles 0 0 90) true
	sliderTime = frame_n
)
--biped.setTransform o #rotation (quat 0 0 0 0) true

fn write_bip_state o f=
(
	q = biped.getTransform o #rotation
	if(q!= undefined) then
	(
		eu = quatToEuler q 
		--WriteFloat f eu.x
		--WriteFloat f eu.y
		--WriteFloat f eu.z
		--WriteFloat f q.w
		WriteFloat f q.x
		WriteFloat f q.y
		WriteFloat f q.z
		WriteFloat f q.w
		
	)
	else
	(
		WriteFloat f -1
		WriteFloat f -1
		WriteFloat f -1
		WriteFloat f -1
	)
	--p = biped.getParentNodePos o
	p = biped.getTransform o #pos
	if(p!= undefined) then
	(
		WriteFloat f p.x
		WriteFloat f p.y
		WriteFloat f p.z
	)
	else
	(
		WriteFloat f -1
		WriteFloat f -1
		WriteFloat f -1
	)
)

-- write biped animation to a file
fname=(APP5_PATH + "biped_walk.bip")
f=fopen fname "wb"	

n = 0
for o in objects do 
  if( findString o.name"Bip" != undefined) then
  (
 	write_str256 f o.name
	for frame_n=0 to N_frames do  
	(
		sliderTime = frame_n
		write_bip_state o f
	)	
	
	--padding
	for i=0 to 7 do WriteByte f 0xFA

	  n = n + 1
   )
fclose(f)		

--update objects.ini
-- ss = stringstream "a"
-- format "Biped:biped01{ n=% N_frames=% }" n N_frames to:ss
-- keys = #((ss as string))
-- UpdateIni keys DoReplacef
-- shellLaunch "notepad.exe" (APP5_PATH + "objects.ini")	

   
   
   
   
   
   
-- Obtain the subcontrollers
----horzCont = bip.horizontal.controller
--turnCont = bip.turning.controller

-- Get the first key for each subcontroller
--vk = biped.getKey vertCont 1 
--hk = biped.getKey horzCont 1 
--tk = biped.getKey turnCont 1 

--print vk.z
-- Show the properties for the individual subcontroller key types 

--showProperties vk 
--showProperties hk 
--showProperties tk   
-- Obtain the Biped’s Footstep controller 
--fsCont = $'Bip01 Footsteps'.transform.controller   
-- Show the Footstep controller properties 
--showProperties fsCont   -- Get the first Footstep controller key 
--fk = biped.getkeyfsCont 1 
--showProperties fk 