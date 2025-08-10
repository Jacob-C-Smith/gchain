# syntax=docker/dockerfile:1

FROM fedora

RUN dnf -y install git clang make net-tools hostname

WORKDIR /home
# COPY ./certificates /home/certificates
COPY ./ /home/
# COPY ./lib /home/lib
RUN (cd /home/; make)
RUN (cd /home/; chmod +x fork.sh)
CMD (cd /home/; ./fork.sh)