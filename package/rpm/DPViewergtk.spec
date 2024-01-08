Name:			DPViewergtk
Version:		0.0.1
Release:		1%{?dist}
Summary:		Software design patterns viewer
License:		GPL-3.0-or-later
Group:			Applications/Education
URL:			https://github.com/tclsteixeira/design-patterns-viewer
Source:			%{url}/raw/main/archive/%{name}-%{version}.tar.gz

BuildRequires:	gcc
BuildRequires:	make
BuildRequires:	gtk3 >= 3.x
BuildRequires:	gtksourceview3
BuildRequires:	sqlite
BuildRequires:	gettext

# Add the following lines to include the man page
%define section 1
%define section_name %{name}

%description
From a list of software design patterns you can view the description of the pattern, a code example, its frequency of use, the participant entities and real world usage cases examples in a single window.
 
%prep
%autosetup

%build
cd Debug
make clean
make main-build

%install

# Create necessary directories based on user-defined prefix
install -d %{buildroot}/%{_prefix}/bin
install -d %{buildroot}/%{_datadir}/%{name}
install -d %{buildroot}/%{_localstatedir}/%{name}/database
install -d %{buildroot}/%{_datadir}/%{name}/images
install -d %{buildroot}/%{_datadir}/applications
install -d %{buildroot}/%{_datadir}/icons/hicolor/scalable/apps

# Install binary without debug information
install -m 755 Debug/%{name} %{buildroot}/%{_prefix}/bin

# Install data files
install -m 644 images/%{name}.svg %{buildroot}/%{_datadir}/icons/hicolor/scalable/apps/
install -m 644 images/logo.svg %{buildroot}/%{_datadir}/%{name}/images/
install -m 644 database/dpviewerdb.sqlite %{buildroot}/%{_localstatedir}/%{name}/database/
install -m 644 %{name}.desktop %{buildroot}/%{_datadir}/applications/

# Install translated language files (*.mo)
install -d %{buildroot}%{_datadir}/locale/
for file in $(find locale -name '*.mo' -not -name '*.pot' -not -name '*.po'); do
    dest_dir=$(dirname %{buildroot}%{_datadir}/$file)
    install -D -m 644 "$file" "$dest_dir/$(basename $file)"
done

# Install the man page to the appropriate location
install -Dm 644 %{section_name}.%{section} %{buildroot}%{_mandir}/man%{section}/%{section_name}.%{section}.gz


%files 
%license COPYING
%doc README AUTHORS ChangeLog
%{_prefix}/bin/%{name}
%{_datadir}/%{name}/
%{_localstatedir}/%{name}/database/
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg
%{_datadir}/locale/*
%{_mandir}/man%{section}/%{section_name}.%{section}.gz

%changelog
* Wed Jan 03 2024 Tiago C. Teixeira <xpto@example.com> - 0.0.1
- Initial RPM release
