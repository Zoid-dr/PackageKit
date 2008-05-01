/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007-2008 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <sys/wait.h>
#include <fcntl.h>

#include <glib/gi18n.h>

#include "pk-debug.h"
#include "pk-network_nm.h"
#include "pk-marshal.h"

static void     pk_network_nm_class_init	(PkNetworkNmClass *klass);
static void     pk_network_nm_init		(PkNetworkNm      *network_nm);
static void     pk_network_nm_finalize		(GObject          *object);

#define PK_NETWORK_NM_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), PK_TYPE_NETWORK_NM, PkNetworkNmPrivate))

/**
 * PkNetworkNmPrivate:
 *
 * Private #PkNetworkNm data
 **/
struct _PkNetworkNmPrivate
{
	guint			 callback_id;
};

enum {
	PK_NETWORK_NM_STATE_CHANGED,
	PK_NETWORK_NM_LAST_SIGNAL
};

static guint signals [PK_NETWORK_NM_LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (PkNetworkNm, pk_network_nm, G_TYPE_OBJECT)

/**
 * pk_network_nm_get_network_state:
 * @network_nm: a valid #PkNetworkNm instance
 *
 * Return value: always %TRUE - this method should never be called
 **/
PkNetworkEnum
pk_network_nm_get_network_state (PkNetworkNm *network_nm)
{
	g_return_val_if_fail (PK_IS_NETWORK_NM (network_nm), PK_NETWORK_ENUM_UNKNOWN);
	/* don't do any checks */
	return PK_NETWORK_ENUM_ONLINE;
}

/**
 * pk_network_nm_class_init:
 * @klass: The PkNetworkNmClass
 **/
static void
pk_network_nm_class_init (PkNetworkNmClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = pk_network_nm_finalize;
	signals [PK_NETWORK_NM_STATE_CHANGED] =
		g_signal_new ("state-changed",
			      G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST,
			      0, NULL, NULL, g_cclosure_marshal_VOID__UINT,
			      G_TYPE_NONE, 1, G_TYPE_UINT);
	g_type_class_add_private (klass, sizeof (PkNetworkNmPrivate));
}

/**
 * pk_network_nm_init:
 * @network_nm: This class instance
 **/
static void
pk_network_nm_init (PkNetworkNm *network_nm)
{
	network_nm->priv = PK_NETWORK_NM_GET_PRIVATE (network_nm);
}

/**
 * pk_network_nm_finalize:
 * @object: The object to finalize
 **/
static void
pk_network_nm_finalize (GObject *object)
{
	PkNetworkNm *network_nm;
	g_return_if_fail (object != NULL);
	g_return_if_fail (PK_IS_NETWORK_NM (object));
	network_nm = PK_NETWORK_NM (object);

	g_return_if_fail (network_nm->priv != NULL);
	G_OBJECT_CLASS (pk_network_nm_parent_class)->finalize (object);
}

/**
 * pk_network_nm_new:
 *
 * Return value: a new PkNetworkNm object.
 **/
PkNetworkNm *
pk_network_nm_new (void)
{
	PkNetworkNm *network_nm;
	network_nm = g_object_new (PK_TYPE_NETWORK_NM, NULL);
	return PK_NETWORK_NM (network_nm);
}

