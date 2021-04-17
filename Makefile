zip:clean
	zip -r ./test3.zip ./ \
		-x ".vscod/*e" \
		-x ".git/*" \
		-x "submodules/*"
	make mv

mv:
	mv ./test3.zip ~/Downloads/

clean:
	rm -f ./test3.zip

