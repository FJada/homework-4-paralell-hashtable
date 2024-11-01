test_%: %.c test.c util.c
	gcc -pthread -o $@ $^

clean:
	rm -f test_*

.PHONY: submission
submission:
	@if [ ! -z "$$(git status --porcelain)" ]; \
		then \
			echo 'Uncommitted work found. Did you forget to run `anubis autosave`?'; \
			git status --porcelain; \
			exit; \
		else \
			git diff $$(git rev-list --max-parents=0 HEAD) > submission.patch; \
			git rev-list HEAD -n 1 > cur_revision; \
			zip $$(pwd)/submission.zip submission.patch cur_revision; \
			echo "Please download and submit $$(pwd)/submission.zip on gradescope :)"; \
	fi