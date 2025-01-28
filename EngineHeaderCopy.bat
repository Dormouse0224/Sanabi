xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\include\Engine"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.inl" ".\External\include\Engine"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Engine\*.fx" ".\Output\Content\HLSL"