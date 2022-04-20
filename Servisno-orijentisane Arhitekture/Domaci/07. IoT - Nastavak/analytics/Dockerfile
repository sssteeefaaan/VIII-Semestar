FROM mcr.microsoft.com/dotnet/sdk:5.0-buster-slim AS build
WORKDIR /src
COPY analytics.csproj .
COPY . .
RUN dotnet restore
RUN dotnet publish -c release -o /app

FROM mcr.microsoft.com/dotnet/aspnet:5.0-buster-slim AS base
WORKDIR /app
COPY --from=build /app .
ENTRYPOINT ["dotnet", "analytics.dll"]