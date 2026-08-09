# C-Skeleton
Just trying to follow Zed on LCTHW.


# Run in docker
I tried a custom variant of a make file on Windows since the test building 
of Zed's example did not work and made me angry.

However, it did not work out, I rather use linux for all of it:
Resp. a docker container:

```
Make sure Docker Desktop daemon runs.

docker run -it --volume <repopath>:/tmp/app llvm-neovim:latest

# cd /tmp/app/clib
# make

```
