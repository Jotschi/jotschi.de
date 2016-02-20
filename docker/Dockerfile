FROM debian:latest
#FROM nginx:1.9.1

RUN apt-get update ; apt-get upgrade ; apt-get dist-upgrade
RUN apt-get -y install git-core jekyll nginx ruby-redcloth cron
RUN cd /opt/ ; git clone https://github.com/Jotschi/jotschi.de.git ; cd /opt/jotschi.de ; jekyll build
RUN ln -s /opt/jotschi.de/hooks/post-merge /opt/jotschi.de/.git/hooks/post-merge
RUN ls -la /opt/jotschi.de/.git/hooks
RUN mkdir -p /etc/nginx/sites-extra ; cp /opt/jotschi.de/_site/wordpress*.conf /etc/nginx/sites-extra
RUN rm /etc/nginx/sites-enabled/*
ADD blog /etc/nginx/sites-enabled/

RUN useradd -ms /bin/bash git
RUN chown git: /opt/jotschi.de -R
RUN touch /var/log/cron.log && chown git: /var/log/cron.log
ADD git-pull /etc/cron.d/git-pull
RUN chmod 0644 /etc/cron.d/git-pull

RUN echo "\ndaemon off;" >> /etc/nginx/nginx.conf
WORKDIR /etc/nginx

CMD cron && nginx && tail -f /var/log/cron.log

EXPOSE 80
EXPOSE 443


