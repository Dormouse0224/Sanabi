del /s /q ".\External\include\Engine"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\include\Engine"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.inl" ".\External\include\Engine"

del /s /q ".\Output\Content\HLSL\Engine"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.fx" ".\Output\Content\HLSL\Engine"