# DataPersistence - 반도체 재고 관리 DB 연동 POC

## 프로젝트 개요

반도체 재고 관리 프로그램의 데이터 영속성(Data Persistence) 구현을 위한 데이터베이스 연동 POC(Proof of Concept) 프로젝트입니다.

## 목적

- 기존 인메모리 방식의 재고 데이터를 영구 저장소(DB)와 연동
- CRUD 기반 데이터 영속성 검증
- 실제 시스템 도입 전 기술 스택 및 연동 방식 검증

## 기술 스택

- Language: C++
- IDE: Visual Studio
- DB: TBD (POC 진행 중)

## 프로젝트 구조

```
DP/
├── DP.slnx          # Visual Studio 솔루션 파일
└── DP/
    ├── DP.vcxproj   # Visual Studio 프로젝트 파일
    └── ...
```

## 개발 환경 설정

1. Visual Studio 설치 (C++ 워크로드 포함)
2. 저장소 클론
   ```bash
   git clone git@github.com:dhkang8828/DataPersistence-KDH.git
   ```
3. `DP.slnx` 파일을 Visual Studio로 열기

## 진행 상황

- [x] 프로젝트 초기 설정
- [ ] DB 연동 구현
- [ ] CRUD 기능 구현
- [ ] 테스트 및 검증
