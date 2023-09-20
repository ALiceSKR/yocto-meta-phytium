# Released under the MIT license (see COPYING.MIT for the terms)

CURDIR := "${THISDIR}/"
PATCHPATH ?= "${@d.getVar('CURDIR') + d.getVar('BPN') + '_' + d.getVar('PV')}"

python () {
    bb.debug(1,"MUSKR ==================================== CURDIR : %s BPN:%s PV:%s",d.getVar('CURDIR'),d.getVar('BPN') ,d.getVar('PV') )
    dir = d.getVar('PATCHPATH') or ''
    if not os.path.isdir(dir):
        return

    bb.parse.mark_dependency(d, dir)

    files = os.listdir(dir)
    files.sort()
    for file in files:
        if file.endswith('.patch'):
            d.appendVar('SRC_URI', ' file://' + dir + '/' + file)
            bb.debug(2, 'Adding patch: ' + file + ' for ' + dir)
}
