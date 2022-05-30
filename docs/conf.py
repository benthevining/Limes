# -*- coding: utf-8 -*-
# pylint: skip-file
# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

source_suffix = '.rst'
root_doc = 'index'

project = 'Limes'
author = 'Ben Vining'
copyright = '2022 Ben Vining; released to public ownership under the terms of the GNU Public License'
version = '@conf_version@'  # feature version
release = '@conf_version@'  # full version string

language = 'en'

needs_sphinx = '4.1'

nitpicky = True
smartquotes = True
numfig = False

#
# extensions

# editorconfig-checker-disable
extensions = ['sphinx.ext.autosectionlabel', 'sphinx.ext.githubpages']
# editorconfig-checker-enable

#
# man pages

man_show_urls = False
man_make_section_directory = False

#
# HTML

# html_baseurl
html_show_sourcelink = False
html_theme = 'default'
html_split_index = True
html_copy_source = False
html_scaled_image_link = True

# editorconfig-checker-disable
html_sidebars = {
    '**':
    ['localtoc.html', 'relations.html', 'globaltoc.html', 'searchbox.html']
}
# editorconfig-checker-enable

html_last_updated_fmt = '%b %d, %Y'

html_permalinks = True

html_title = f'Limes {release} Documentation'
html_short_title = f'Limes {release} Documentation'
# html_favicon

#
# Latex

latex_show_pagerefs = True
latex_show_urls = 'footnote'

#
# Texinfo

texinfo_show_urls = 'footnote'

#
# Linkcheck

linkcheck_retries = 4
