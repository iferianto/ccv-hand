
:: load the vars file to get access to vcbuild

echo loading variables

call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"

for /d %%X in (..\apps\*) do ( 

cd %%X

	for /d %%Y in (*) do ( 
		cd %%Y
		
		for %%Z in (*.sln) do (
		
			echo trying comipilation
			
			vcbuild /na /nd --build %%Z
		)
		cd ../
	)


cd ../
)

cd ../scripts