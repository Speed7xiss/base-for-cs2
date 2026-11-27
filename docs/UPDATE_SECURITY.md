# Update and schema security

## Scope
The schema registry versions offline export formats. Its keys map logical fields
to CSV column names. They are not CS2 process offsets, memory addresses,
module-relative addresses, or live-game data sources.

## Update pipeline
The updater policy checks product, channel, numeric version, HTTPS URL,
manifest completeness, publisher signature, download result, and package
SHA-256 before asking the installer to stage a release. The signed UTF-8
payload uses these fields in this exact order, separated by LF and terminated
by LF: product, version, channel, artifact_url, sha256, minimum_version.

The application must pin a trusted Ed25519 public key out-of-band. Never trust
a key delivered by the update manifest. The transport adapter must validate
TLS certificates and hostnames and must not follow redirects to non-HTTPS URLs.
The trust adapter must implement real Ed25519 and SHA-256. The installer must
use temporary staging, reject path traversal and unexpected files, and perform
an atomic switch with rollback.

## Implementation boundary
This commit supplies the schema registry and updater policy interfaces. It
does not ship a concrete network client, Ed25519/SHA-256 provider, package
format, or platform installer. Automatic updates must remain disabled until
those adapters exist and have been tested.
