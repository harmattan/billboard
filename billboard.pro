TEMPLATE = subdirs
SUBDIRS += billboardd billboard-ui

css.path = /opt/billboard/css
css.files = css/libsysuid-screenlock-nokia.css.billboard css/libsysuid-screenlock-nokia.css.original
INSTALLS += css

cli.path = /opt/billboard/bin
cli.files = billboard-cli/billboard-cli
INSTALLS += cli

# Update version numbers
system(sh version.sh)
