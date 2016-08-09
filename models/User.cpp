#include "User.h"
#include "Role.h"
#include "Issue.h"

#include <system/database/query/QueryBuilder.h>

using namespace tuber;

User::User(){
	this->setModelAttributes("users","user_id",true);
}

string User::getAuthIdentifierColumnName(){
	return "username";
}

string User::getAuthPasswordColumnName(){
	return "password";
}

string User::getAuthIdentifier(){
	if(this->m_original != nullptr && this->m_original->hasString(getAuthIdentifierColumnName())){
		return this->m_original->getString(getAuthIdentifierColumnName());
	}
	if(this->m_attributes != nullptr && this->m_attributes->hasString(getAuthIdentifierColumnName())){
		return this->m_attributes->getString(getAuthIdentifierColumnName());
	}
	return "";
}

string User::getAuthPassword(){
	if(this->m_original != nullptr && this->m_original->hasString(getAuthPasswordColumnName())){
		return this->m_original->getString(getAuthPasswordColumnName());
	}
	if(this->m_attributes != nullptr && this->m_attributes->hasString(getAuthPasswordColumnName())){
		return this->m_attributes->getString(getAuthPasswordColumnName());
	}
	return "";
}

int User::getAuthId(){
	return this->getKey();
}

void User::clearAuthAttr(){
	if(this->m_original != nullptr){
		this->m_original->reset();
	}
	if(this->m_attributes != nullptr){
		this->m_attributes->reset();
	}
}

void User::syncAuthAttr(QueryRow row){
	this->fill(row);
	this->syncOriginal();
}

void User::getIssueableUsers(RowCallback callback,Bundle* data){
	User::select({"users.user_id",
				"users.first_name",
				"users.middle_name",
				"users.last_name",
				"COUNT(issues.issue_id) AS issue_count"})
		->from("users")
		->join("issues","users.user_id = issues.user_id","left")
		->where("users.status_id","=",1)
		->where("users.role_id","=",Role::ROLE_STUDENT)
		->groupBy("users.user_id")
		->having("issues.issue_id IS NULL or (issues.issue_id IS NOT NULL and (issues.status_id = 1 and COUNT(issues.issue_id) BETWEEN 0 AND 4) or (issues.status_id != 1))")
		->get(callback,data);
}

void User::getValidUsers(RowCallback callback,Bundle* data){
	User::select({
		"users.user_id",
		"users.first_name",
		"users.middle_name",
		"users.last_name",
		"users.status_id",
		"roles.role_id",
		"roles.role_name"
	})
	->from("users")
	->join("roles","roles.role_id = users.role_id")
	->where("users.status_id","=",STATUS_VALID)
	->orderBy("users.last_name")
	->get(callback,data);
}

void User::getUsersForId(vector<int> ids,RowCallback callback,Bundle* data){
	QueryBuilder* builder = User::select({
		"users.user_id",
		"users.first_name",
		"users.middle_name",
		"users.last_name",
		"users.status_id",
		"roles.role_id",
		"roles.role_name"
	})
	->from("users")
	->join("roles","roles.role_id = users.role_id");

	for(auto id: ids){
		builder->where("users.user_id","=",id,"or");
	}

	builder->where("users.status_id","=",STATUS_VALID)
			->orderBy("users.last_name")
			->get(callback,data);
}
