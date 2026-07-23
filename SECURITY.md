# Security Policy

> [!IMPORTANT]
> Do **not** report vulnerabilities in public GitHub issues!

## Reporting a Vulnerability

1. Preferred: GitHub private vulnerability reporting
   - Repository page -> **Security** -> **Report a vulnerability** -> **Submit report**.
2. Fallback: contact the maintainer directly and include "[SECURITY]" in the subject.

## What Is a Security Issue

An issue is considered security-related if it can impact confidentiality, integrity, or availability.

Typical security issues:

- Memory safety bugs that may lead to code execution or compromise.
- Denial-of-service from untrusted input (crash, hang, resource exhaustion).
- Unintended file, path, or privilege access.
- Supply chain or build/packaging weaknesses in this repository.

Usually not security issues:

- Crashes or warnings without security impact.
- Performance, compatibility, or API ergonomics issues.
- General feature requests.

## What to Include

Please include:

- Impact and affected versions/platforms/toolchain.
- Reproduction steps or a minimal proof of concept.
- Any known mitigations and whether the issue is already public.

## Supported Versions

Security fixes are provided for the latest release and the default branch.

| Version | Supported |
| ------- | --------- |
| Latest release | :white_check_mark: |
| Default branch (`master/main`) | :white_check_mark: |
| Older releases | :x: |

## Response Process

Best-effort targets:

- Initial acknowledgment: within 10 days.
- Triage decision: within 20 days.
- Status updates: at least every 20 days while the issue is open.

Timelines may vary with complexity and maintainer availability.

## Disclosure Policy

Please allow time for investigation and a coordinated fix before public disclosure.
We follow coordinated disclosure and credit reporters who want acknowledgment.

## Scope

In scope:

- Source code in this repository.
- Build and packaging definitions in this repository.

Out of scope:

- Issues only affecting unsupported, modified, or end-of-life forks.
- General usage questions, feature requests, or non-security bugs (use GitHub issues for those).
