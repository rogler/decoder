/*
 * vim: set tabstop=4 syntax=c :
 *
 * Copyright (C) 2014-2017, Peter Haemmerlein (peterpawn@yourfritz.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program, please look for the file LICENSE.
 */

#define DEVPW_C

#include "common.h"

static commandEntry_t 		__devpw_command = { .name = "device_password", .ep = &devpw_entry, .usage = &devpw_usage };
EXPORTED commandEntry_t *	devpw_command = &__devpw_command;

// display usage help

void 	devpw_usage(bool help)
{
	errorMessage("help for device_password\n");
	if (help)
		errorMessage("option --help used\n");
}

// 'device_password' function - compute the password hash from the specified device properties

int		devpw_entry(int argc, char** argv, int argo, commandEntry_t * entry)
{
	bool				hexOutput = false;
	char				hash[MAX_DIGEST_SIZE];
	size_t				hashLen = sizeof(hash);
	char				hex[(sizeof(hash) * 2) + 1];
	char *				out;
	size_t				outLen;
	char *				serial = NULL;
	char *				maca = NULL;
	char *				wlanKey = NULL;
	char *				tr069Passphrase = NULL;

	if (argc > argo + 1)
	{
		int				opt;
		int				optIndex = 0;

		static struct option options_long[] = {
			verbosity_options_long,
			{ "hex-output", no_argument, NULL, 'x' },
		};
		char *			options_short = "x" verbosity_options_short;

		while ((opt = getopt_long(argc - argo, &argv[argo], options_short, options_long, &optIndex)) != -1)
		{
			switch (opt)
			{
				case 'x':
					hexOutput = true;
					break;

				check_verbosity_options_short();
				help_option();
				getopt_message_displayed();
				invalid_option(opt);
			}
		}
		if (optind >= (argc - argo))
		{
			errorMessage("Missing password on command line.\a\n");
			return EXIT_FAILURE;
		}
		else
		{
			int		i = optind + argo;
			int		index = 0;

			char *	*arguments[] = {
				&serial,
				&maca,
				&wlanKey,
				&tr069Passphrase,
				NULL
			};

			while (argv[i])
			{
				*(arguments[index++]) = argv[i++];
				if (!arguments[index])
					break;
			}
			if (!maca)
			{
				errorMessage("At least two arguments (serial and maca) are required.\a\n");
				__usage(false);
				return EXIT_FAILURE;
			}
		}
	}
	else
	{
		errorMessage("Missing arguments on command line.\a\n");
		__usage(false);
		return EXIT_FAILURE;
	}

	resetError();

	if (keyFromProperties(hash, &hashLen, serial, maca, wlanKey, tr069Passphrase))
	{
		if (hexOutput)
		{
			outLen = binaryToHexadecimal((char *) hash, hashLen, hex, sizeof(hex));
			out = hex;
		}
		else
		{
			outLen = hashLen;
			out = (char *) hash;
		}
		if (fwrite(out, outLen, 1, stdout) != 1)
		{
			errorMessage("Write to STDOUT failed.\a\n");
		}
	}
	else
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}