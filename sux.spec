Name:           sux
Version:        1.01
Release:        0%{?dist}
Summary:        Sort/Uniq eXtended

Group:          System Environment/Base
License:        BSD
URL:            http://sysctl.org/sux/
Source0:        http://sysctl.org/sux/d/sux-1.01.tgz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  binutils coreutils diffutils file filesystem findutils gcc glibc-devel glibc-headers grep gzip libstdc++-devel make mktemp prelink

%description
If you like to pipe sort to uniq, you will love Sux.

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README
%{_bindir}/sux

%changelog
* Thu Dec 19 2013 - Pierre Ynard - 1.01-0
- Fix a small display bug

* Mon Apr 30 2012 - Philippe Bourcier - philippe -at- sysctl.org 1.0-0
- Initial version of the package

