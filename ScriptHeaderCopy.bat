del /s /q ".\External\include\SanabiScript"
xcopy /s /y /exclude:exclude_list.txt ".\Project\SanabiScript\*.h" ".\External\include\SanabiScript"
xcopy /s /y /exclude:exclude_list.txt ".\Project\SanabiScript\*.inl" ".\External\include\SanabiScript"