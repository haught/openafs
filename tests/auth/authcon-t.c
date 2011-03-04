/*
 * Copyright (c) 2010 Your File System Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR `AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * Test those bits of the authcon interface that we can test without involving
 * the cache manager.
 */

#include <afsconfig.h>
#include <afs/param.h>

#include <roken.h>

#include <rx/rx.h>
#include <rx/rxkad.h>
#include <afs/cellconfig.h>

#include <tap/basic.h>
#include "common.h"

int
main(int argc, char **argv)
{
    struct afsconf_dir *dir;
    char *dirname;
    struct rx_securityClass **classes;
    int numClasses;

    plan(3);
    dirname = buildTestConfig();

    dir = afsconf_Open(dirname);
    if (dir == NULL) {
	fprintf(stderr, "Unable to configure directory.\n");
        exit(1);
    }

    /* Server Security objects */

    afsconf_BuildServerSecurityObjects(dir, &classes, &numClasses);
    is_int(3, numClasses, "3 security classes are returned, as expected");
    ok(classes[1] == NULL, "The rxvab class is undefined, as requested");
    free(classes);

    afsconf_SetSecurityFlags(dir, AFSCONF_SECOPTS_ALWAYSENCRYPT);

    afsconf_BuildServerSecurityObjects(dir, &classes, &numClasses);
    is_int(4, numClasses, "When encryption is enabled, 4 classes are returned");

    return 0;
}
