api = 2
core = 7.x
exclusive = TRUE

; Core project
; ------------
projects[drupal][type] = core
projects[drupal][version] = 7.54

; Include any modules, themes, and libraries that are hosted remotely.

; Modules
; --------
; Include any modules that are hosted remotely.

projects[ctools][version] = 1.12
projects[ctools][type] = module

projects[date][version] = 2.9
projects[date][type] = module

projects[entity][version] = 1.8
projects[entity][type] = module

projects[entitycache][version] = 1.5
projects[entitycache][type] = module

projects[features][version] = 2.10
projects[features][type] = module

projects[plug][version] = 1.1
projects[plug][type] = module

projects[registry_autoload][version] = 1.3
projects[registry_autoload][type] = module

projects[restful][version] = 2.14
projects[restful][type] = module

projects[strongarm][version] = 2.0
projects[strongarm][type] = module

projects[jquery_update][version] = 2.7
projects[jquery_update][type] = module

projects[libraries][version] = 2.3
projects[libraries][type] = module

projects[link][version] = 1.4
projects[link][type] = module

projects[media][version] = 1.6
projects[media][type] = module

projects[entityreference][version] = 1.2
projects[entityreference][type] = module

;projects[field_collection][version] = 1.0-beta12
;projects[field_collection][type] = module

;projects[field_group][version] = 1.5
;projects[field_group][type] = module

;projects[hierarchical_select][version] = 3.0-alpha6
;projects[hierarchical_select][type] = module

;projects[i18n][version] = 1.14
;projects[i18n][type] = module

;projects[login_security][version] = 1.9
;projects[login_security][type] = module

;projects[memcache][version] = 1.5
;projects[memcache][type] = module

;projects[metatag][version] = 1.18
;projects[metatag][type] = module

;projects[migrate][version] = 2.8
;projects[migrate][type] = module

;projects[multiupload_filefield_widget][version] = 1.13
;projects[multiupload_filefield_widget][type] = module

;projects[multiupload_imagefield_widget][version] = 1.3
;projects[multiupload_imagefield_widget][type] = module

;projects[password_policy][version] = 2.0-alpha6
;projects[password_policy][type] = module

;projects[r4032login][version] = 1.8
;projects[r4032login][type] = module

;projects[state_machine][version] = 2.6
;projects[state_machine][type] = module

;projects[table_element][version] = 1.0-beta5
;projects[table_element][type] = module

;projects[token][version] = 1.6
;projects[token][type] = module

;projects[variable][version] = 2.5
;projects[variable][type] = module

;projects[view_unpublished][version] = 1.2
;projects[view_unpublished][type] = module

;projects[xautoload][version] = 4.5
;projects[xautoload][type] = module

;projects[ultimate_cron][version] = 2.0
;projects[ultimate_cron][type] = module

;projects[url][patch][] = "https://drupal.org/files/0001-entity_metadata_wrapper.patch"

;projects[views][version] = 3.11
;projects[views][type] = module


; Development Modules
projects[devel][version] = 1.5
projects[devel][type] = module

;projects[diff][version] = 3.2
;projects[diff][type] = module

projects[module_filter][version] = 2.0
projects[module_filter][type] = module


; Features
; ---------
; Include any features that are hosted remotely.

; Themes
; --------
; Include any themes that are hosted remotely.

;projects[bootstrap][version] = 3.10
;projects[bootstrap][type] = theme

; Libraries
; ---------
; Include libaries required by the above projects.
