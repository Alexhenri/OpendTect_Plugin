#_______________________Pmake___________________________________________________
#
#	Makefile : 	Multi-Machine processing
# 	July 2008	Bert
#	RCS :		$Id$
#_______________________________________________________________________________


add_definitions( -DmNoAutoUiString )

set(OD_MODULE_DEPS Network)
set(OD_USEQT Core)

SET(OD_MODULE_SOURCES
	jobdescprov.cc
	jobiomgr.cc
	jobrunner.cc
	initmmproc.cc
	clusterjobdispatch.cc
	mmbatchjobdispatch.cc
	mmproc.cc
	mmexecproxy.cc
	remcommhandler.cc
	remjobexec.cc
)

SET(OD_MODULE_PROGS
	od_remoteservice.cc
	od_remexec.cc
)

OD_INIT_MODULE()
