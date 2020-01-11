Name:       flatpak-runner

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Flatpak runner
Version:    0.3.0
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/rinigus/flatpak-runner
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   rsync
Requires:   flatpak
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Compositor)


%description
Wrapper for running Flatpak apps on Sailfish.


%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install
chmod 755 %{buildroot}/usr/bin/flatpak-extension-hybris

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
